#pragma once
// C include
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

// My include
#include "Image_jpeg.h"

// 图片类型
#define IMAGE_JPEG  0x1
#define IMAGE_BMP   0x2
#define IMAGE_PNG   0x4

// 初始化宏
#define IMAGE_INIT_ALL  0xFFFFFFFF
#define IMAGE_INIT_JPEG IMAGE_JPEG
#define IMAGE_INIT_BMP  IMAGE_BMP
#define IMAGE_INIT_PNG  IMAGE_PNG

// 图像数据
struct IMAGE
{
    void* Pixel;            // 像素
    unsigned int    Width,  // 宽度
                    Hight,  // 高度
                    Depth;  // 颜色深度
};

extern

/**
 * 初始化IMAGE格式
 *
 * \param Type  : 初始化类型
 *
 * \returns 0为初始化成功,否则为相应的格式错误
 */
unsigned int ImageInit(unsigned int Type);

/**
 * 从文件读取数据
 *
 * \param File  : 源文件
 * \param image : 储存对象
 *
 * \returns 读取结果
 */
BOOL ImageLoadFormFile(const char* File, struct IMAGE *image);

/**
 * 从内存读取数据
 *
 * \param Data  : 源数据
 * \param image : 储存对象
 *
 * \returns 读取结果
 */
BOOL ImageLoadFormMem(const void* Data, struct IMAGE* image);

/**
 * 保存到文件
 *
 * \param File      : 目标文件名
 * \param image     : 储存对象
 * \param Type      : 保存类型
 * \param quality   : 压缩率
 *
 * \returns 保存结果
 */
BOOL ImageSaveToFile(char* File, const struct IMAGE* image, unsigned int Type, int quality);

/**
 * 保存到内存
 *
 * \param Data      : 目标数据指针
 * \param image     : 储存对象
 * \param Type      : 保存类型
 * \param quality   : 压缩率
 *
 * \returns 0保存失败，否则为数据大小
 */
unsigned int ImageSaveToMem(void** Data, const struct IMAGE*image, unsigned int Type, int quality);