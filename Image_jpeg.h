#pragma once
#include <setjmp.h>
#include <windows.h>
#include "jpeglib.h"
#pragma comment(lib, "jpeg.lib")

// ����
struct Jpeg_Error
{
    struct jpeg_error_mgr pub;  /* "public" fields */
    jmp_buf setjmp_buffer;    /* for return to caller */
};

/**
 * ��ʼ��Jpeg
 *
 * \param NULL
 *
 * \returns �Ƿ��ʼ���ɹ�
 */
BOOL Jpeg_Init();

/**
 * ��ȡjpeg
 *
 * \param *Data     : Դjpeg����
 * \param *image    : ����IMAGE����
 *
 * \returns �Ƿ��ȡ�ɹ�
 */
BOOL Jpeg_Load(const void*Data, struct IMAGE *image);

/**
 * ����jpeg
 *
 * \param ** Data   : Ҫ�����ָ��
 * \param *image    : ͼ���ָ��
 * \param quality   : ѹ����(0-100)0Ϊ���
 *
 * \returns ���ݴ�С,������0���ȡ����
 */
unsigned int Jpeg_Save(void** Data, const struct IMAGE* image, int quality);