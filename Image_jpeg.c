#include "image.h"

inline METHODDEF(void) my_error_exit(j_common_ptr cinfo);

/**
 * 初始化Jpeg
 *
 * \param NULL
 *
 * \returns 是否初始化成功
 */
BOOL Jpeg_Init()
{

    return TRUE;
}

/**
 * 读取jpeg
 *
 * \param *Data     : 源jpeg数据
 * \param *image    : 保存IMAGE对象
 *
 * \returns 是否读取成功
 */
BOOL Jpeg_Load(const void* Data, struct IMAGE* image)
{
    struct jpeg_decompress_struct cinfo;
    struct Jpeg_Error jerr;

    /*设置错误处理*/
    cinfo.err = jpeg_std_error(&jerr.pub);
    jerr.pub.error_exit = my_error_exit;

    /*设置错误处理*/
    if (setjmp(jerr.setjmp_buffer))
    {
        jpeg_destroy_decompress(&cinfo);
        return FALSE;
    }

    // 创建内存图像
    jpeg_create_decompress(&cinfo);
    jpeg_mem_src(&cinfo, (const unsigned char*)Data, _msize((void*)Data));
    // 读取图像信息
    jpeg_read_header(&cinfo, TRUE);
    cinfo.out_color_space = JCS_RGB;
    // 设置信息
    image->Width = cinfo.image_width;
    image->Hight = cinfo.image_height;
    image->Depth = cinfo.num_components;
    // 申请空间
    image->Pixel = (void*)malloc(cinfo.image_width * cinfo.image_height * cinfo.num_components);

    // 读取图像
    jpeg_start_decompress(&cinfo);
    JSAMPROW buffer;
    while (cinfo.output_scanline < cinfo.output_height)
    {
        buffer = (JSAMPROW) & ((char*)image->Pixel)[(cinfo.output_height - cinfo.output_scanline - 1) * cinfo.image_width * cinfo.num_components];
        jpeg_read_scanlines(&cinfo,
            (JSAMPARRAY)&buffer,
            1);
    }
    
    // 清除数据
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    
    return TRUE;
}

/**
 * 保存jpeg
 *
 * \param ** Data   : 要保存的指针
 * \param *image    : 图像的指针
 * \param quality   : 压缩率(0-100)0为最大
 *
 * \returns 数据大小,若返回0则读取错误
 */
unsigned int Jpeg_Save(void** Data, const struct IMAGE* image, int quality)
{
    struct jpeg_compress_struct cinfo;
    struct Jpeg_Error jem;
    unsigned int Size = 0;

    /*设置错误处理*/
    cinfo.err = jpeg_std_error(&jem.pub);
    jem.pub.error_exit = my_error_exit;

    /*设置错误处理*/
    if (setjmp(jem.setjmp_buffer))
    {
        free(*Data);
        *Data = NULL;
        jpeg_destroy_compress(&cinfo);
        return 0;
    }

    // 创建压缩对象
    jpeg_create_compress(&cinfo);
    jpeg_mem_dest(&cinfo, (unsigned char**)Data, &Size);
    // 设置压缩对象
    cinfo.image_width = image->Width;
    cinfo.image_height = image->Hight;
    cinfo.input_components = image->Depth;
    cinfo.in_color_space = JCS_RGB;
    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, quality, TRUE);
    // 开始压缩
    jpeg_start_compress(&cinfo, TRUE);

    JSAMPROW buffer;

    // 对每一行进行压缩
    while (cinfo.next_scanline < cinfo.image_height) {
        
        buffer = &(((char*)image->Pixel)[(cinfo.image_height - cinfo.next_scanline - 1) * cinfo.image_width * cinfo.num_components]);
        jpeg_write_scanlines(&cinfo, (JSAMPARRAY)&buffer, 1);
    }

    // 清除数据
    jpeg_finish_compress(&cinfo);
    jpeg_destroy_compress(&cinfo);

    return Size;
}

// 错误处理
inline METHODDEF(void) my_error_exit(j_common_ptr cinfo)
{
#ifdef _DEBUG
    printf("jpeg格式出现错误!\n");
#endif
    longjmp(((struct Jpeg_Error*)cinfo->err)->setjmp_buffer, 1);
}