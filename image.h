#pragma once
// C include
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

// My include
#include "Image_jpeg.h"

// ͼƬ����
#define IMAGE_JPEG  0x1
#define IMAGE_BMP   0x2
#define IMAGE_PNG   0x4

// ��ʼ����
#define IMAGE_INIT_ALL  0xFFFFFFFF
#define IMAGE_INIT_JPEG IMAGE_JPEG
#define IMAGE_INIT_BMP  IMAGE_BMP
#define IMAGE_INIT_PNG  IMAGE_PNG

// ͼ������
struct IMAGE
{
    void* Pixel;            // ����
    unsigned int    Width,  // ���
                    Hight,  // �߶�
                    Depth;  // ��ɫ���
};

extern

/**
 * ��ʼ��IMAGE��ʽ
 *
 * \param Type  : ��ʼ������
 *
 * \returns 0Ϊ��ʼ���ɹ�,����Ϊ��Ӧ�ĸ�ʽ����
 */
unsigned int ImageInit(unsigned int Type);

/**
 * ���ļ���ȡ����
 *
 * \param File  : Դ�ļ�
 * \param image : �������
 *
 * \returns ��ȡ���
 */
BOOL ImageLoadFormFile(const char* File, struct IMAGE *image);

/**
 * ���ڴ��ȡ����
 *
 * \param Data  : Դ����
 * \param image : �������
 *
 * \returns ��ȡ���
 */
BOOL ImageLoadFormMem(const void* Data, struct IMAGE* image);

/**
 * ���浽�ļ�
 *
 * \param File      : Ŀ���ļ���
 * \param image     : �������
 * \param Type      : ��������
 * \param quality   : ѹ����
 *
 * \returns ������
 */
BOOL ImageSaveToFile(char* File, const struct IMAGE* image, unsigned int Type, int quality);

/**
 * ���浽�ڴ�
 *
 * \param Data      : Ŀ������ָ��
 * \param image     : �������
 * \param Type      : ��������
 * \param quality   : ѹ����
 *
 * \returns 0����ʧ�ܣ�����Ϊ���ݴ�С
 */
unsigned int ImageSaveToMem(void** Data, const struct IMAGE*image, unsigned int Type, int quality);