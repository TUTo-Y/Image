#pragma once
#include <setjmp.h>
#include <windows.h>
#include "jpeglib.h"
#pragma comment(lib, "jpeg.lib")

// 错误
struct Jpeg_Error
{
    struct jpeg_error_mgr pub;  /* "public" fields */
    jmp_buf setjmp_buffer;    /* for return to caller */
};

/**
 * 初始化Jpeg
 *
 * \param NULL
 *
 * \returns 是否初始化成功
 */
BOOL Jpeg_Init();

/**
 * 读取jpeg
 *
 * \param *Data     : 源jpeg数据
 * \param *image    : 保存IMAGE对象
 *
 * \returns 是否读取成功
 */
BOOL Jpeg_Load(const void*Data, struct IMAGE *image);

/**
 * 保存jpeg
 *
 * \param ** Data   : 要保存的指针
 * \param *image    : 图像的指针
 * \param quality   : 压缩率(0-100)0为最大
 *
 * \returns 数据大小,若返回0则读取错误
 */
unsigned int Jpeg_Save(void** Data, const struct IMAGE* image, int quality);