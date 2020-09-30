#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <winsock2.h>
#include <WS2tcpip.h>
#include <windows.h>
#include "openssl/ssl.h"
#include "openssl/err.h"
#include "openssl/x509v3.h"
#include "openssl/rand.h"

#define PORT 1000
#define SERVER "127.0.0.1"
#define CACERT  "./sslca/misc/demoCA/private/ca.crt"
#define MYCERTF "./sslca/misc/demoCA/certs/client.crt"
#define MYKEYF "./sslca/misc/demoCA/private/client.key"
#define MSGLENGTH 1024

int main ()
{
	struct sockaddr_in sin;
	int seed_int[100];
	SSL *ssl;
	SSL_CTX *ctx;
	int i;
	 
	WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
	/* 初始化OpenSSL */
	SSL_library_init();
	/*加载算法库 */
	OpenSSL_add_ssl_algorithms ();
	/*加载错误处理信息 */
	SSL_load_error_strings ();

	/* 创建会话环境 */
	ctx = SSL_CTX_new (TLS_method());
	if (NULL == ctx)
	exit (1);
	/* 制定证书验证方式 */
	SSL_CTX_set_verify (ctx, SSL_VERIFY_PEER, NULL);
	 
	/* 为SSL会话加载CA证书*/
	SSL_CTX_load_verify_locations (ctx, CACERT, NULL);

	/* 为SSL会话加载用户证书 */
	if (0 == SSL_CTX_use_certificate_file (ctx, MYCERTF, SSL_FILETYPE_PEM))
	{
		ERR_print_errors_fp (stderr);
		exit (1);
	}
	/* 为SSL会话加载用户私钥 */
	if (0 == SSL_CTX_use_PrivateKey_file (ctx, MYKEYF, SSL_FILETYPE_PEM))
	{
		ERR_print_errors_fp (stderr);
		exit (1);
	}
	/* 验证私钥和证书是否相符 */
	if (!SSL_CTX_check_private_key (ctx))
	{
		printf ("Private key does not match the certificate public key\n");
		exit (1);
	}
	/* 设置随机数 */
	srand ((unsigned) time (NULL));
	for (i = 0; i < 100; i++)
	seed_int[i] = rand ();
	RAND_seed (seed_int, sizeof (seed_int));
	/* 指定加密器类型 */
	SSL_CTX_set_cipher_list (ctx, "RC4-MD5");
	SSL_CTX_set_mode (ctx, SSL_MODE_AUTO_RETRY);
	int sock;
	printf ("Begin tcp socket...\n");
	/* 创建socket描述符 */
	sock = socket (AF_INET, SOCK_STREAM, 0);
	if (sock == -1)
	{
		printf ("SOCKET error. \n");
		return -1;
	}
	memset (&sin, '\0', sizeof (sin));
	 
	/* 准备通信地址和端口号 */
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = inet_addr (SERVER); /* Server IP */
	sin.sin_port = htons (PORT); /* Server Port number */
	int icnn = connect (sock, (struct sockaddr *) &sin, sizeof (sin));
	if (icnn == -1)
	{
		printf ("can not connect to server,%s\n", strerror (errno));
		exit (1);
	}

	 
	/* 创建一个SSL套接字*/
	ssl = SSL_new (ctx);
	if (NULL == ssl)
	exit (1);

	/* 申请一个ssl套接字 */
	if (0 >= SSL_set_fd (ssl, sock))
	{
		printf ("Attach to Line fail!\n");
		exit (1);
	}
	//建立SSL连接
	int k = SSL_connect (ssl);
	if (0 == k)
	{
		printf ("%d\n", k);
		printf ("SSL connect fail!\n");
		exit (1);
	}
	printf ("connect to server\n");
	char sendmsg[MSGLENGTH] = "\0";
	char revmsg[MSGLENGTH] = "\0";
	//接收服务器消息
	
	while (1)
	{
		printf ("please input the data to send:\n");
		scanf ("%s", sendmsg);
		//向服务器发送消息
		SSL_write (ssl, sendmsg, strlen (sendmsg));
		printf ("send message ' %s ' success\n", sendmsg);
	}
	//关闭连接
	SSL_shutdown (ssl);
	SSL_free (ssl);
	SSL_CTX_free (ctx);
	close (sock);
	getchar ();
	return 0;
}