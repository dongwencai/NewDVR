#ifndef _JVLIVE_CLIENT_H_
#define _JVLIVE_CLIENT_H_

typedef enum{
	JVLIVE_ERR_OK,
	JVLIVE_ERR_OVER,//播放完成
	JVLIVE_ERR_UNKNOWN,
	JVLIVE_ERR_TIMEOUT,
	JVLIVE_ERR_BAD_REQUEST,
	JVLIVE_ERR_WRONG_PASSWD,
	JVLIVE_ERR_NOT_FOUND,

}jvlive_err_e;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * 播放结束，可能主控端结束了播放
 */
typedef void (afterPlayingFunc_t)(void* param, jvlive_err_e errcode, const char *errStr);

/**
 * 数据回调
 *
 *@param param 用户参数
 *@param channelHandle 通道句柄。可用于获取通道相关的信息。主要考虑到，有可能有多路音频，和视频，以及它们的区分
 *@param codecName 编码方式：H264, PCMU, PCMA等
 *@param buffer 帧位置
 *@param frameSize 帧长度
 *@param presentationTime 当前帧时间
 *@param durationInMicroseconds 暂未使用
 */
typedef void (afterGettingFrame_t)(void* param,
								void *channelHandle,
								unsigned char *buffer,
								unsigned frameSize,
								struct timeval presentationTime,
								unsigned durationInMicroseconds);

/**
 * 打开一路码流
 * 注意：本函数不返回，会阻塞，直到调用jvlive_client_stop
 *   如果中途返回，则一定是出错了
 *
 *@param streamID 流名称，唯一标识打开的流
 *@param url 要打开的数据流的地址，类似：rtsp://192.168.11.192:8554/live0.264
 *@param maxVideoStreamSize 视频最大帧的帧大小
 *@param user 用户名，无则为NULL
 *@param passwd 密码，无则为NULL
 *@param afterPlaying 主控结束，或者其它原因导致停止时的回调
 *@param afterGetting 收到数据后的回调函数
 *@param param 用户数据，会在afterPlaying和afterGetting时返回
 */
int jvlive_client_run(
		const char *streamID,
		const char *url,
		int maxVideoStreamSize,
		const char *user,
		const char *passwd,
		afterPlayingFunc_t *afterPlaying,
		afterGettingFrame_t *afterGetting,
		void *param);

/**
 * 关闭之前打开的码流
 */
int jvlive_client_stop(const char *streamID);

typedef struct {
	char mediaName[32];	//video  audio
	char codecName[32]; //H264 PCMA PCMU ...
	char protocolName[32]; //RTP
} jvliveChannelInfo_t;

/**
 * 获取RTSP播放的某码 流具体信息
 *
 *@param channelHandle #afterGettingFrame_t 的参数之一
 *@param info 获取到的信息
 */
int jvlive_client_get_channel_info(void *channelHandle, jvliveChannelInfo_t *info);

#ifdef __cplusplus
}
#endif



#endif
