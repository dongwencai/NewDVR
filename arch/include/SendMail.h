#ifndef _SENDMAIL_H
#define _SENDMAIL_H

#ifdef __cplusplus
extern "C" {
#endif 

#define MAIL_MAX_STR_LEN			128
#define MAIL_MAX_CONTENT_LEN		512
#define MAIL_MAX_RECIPIENT_LEN		32

// TLS/SSL extension
typedef enum
{
	NO_SECURITY,
	USE_TLS,
	USE_SSL,
	DO_NOT_SET
}SMTP_SECURITY_TYPE;

//0  成功
//-1 失败
int SendMail(char pServer[MAIL_MAX_STR_LEN],		//smtp
	unsigned int nPort,							//port
	SMTP_SECURITY_TYPE security,					//验证方式
	char pSender[MAIL_MAX_STR_LEN],					//发件人地址
	char pPWD[MAIL_MAX_STR_LEN],					//密码
	char pSubject[MAIL_MAX_STR_LEN],				//主题
	char pCharset[MAIL_MAX_STR_LEN],				//邮件编码
	char pContent[MAIL_MAX_CONTENT_LEN],			//正文
	int  bIsHtml,									//是否html格式正文
	char pRecipient[][MAIL_MAX_RECIPIENT_LEN],		//收件人
	int  nRecipientCt,								//收件人数量
	char pAttachment[MAIL_MAX_STR_LEN]				//附件
	);

#ifdef __cplusplus
}
#endif 
#endif

