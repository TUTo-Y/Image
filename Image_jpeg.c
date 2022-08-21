#include "image.h"

inline METHODDEF(void) my_error_exit(j_common_ptr cinfo);

/**
 * ��ʼ��Jpeg
 *
 * \param NULL
 *
 * \returns �Ƿ��ʼ���ɹ�
 */
BOOL Jpeg_Init()
{

    return TRUE;
}

/**
 * ��ȡjpeg
 *
 * \param *Data     : Դjpeg����
 * \param *image    : ����IMAGE����
 *
 * \returns �Ƿ��ȡ�ɹ�
 */
BOOL Jpeg_Load(const void* Data, struct IMAGE* image)
{
    struct jpeg_decompress_struct cinfo;
    struct Jpeg_Error jerr;

    /*���ô�����*/
    cinfo.err = jpeg_std_error(&jerr.pub);
    jerr.pub.error_exit = my_error_exit;

    /*���ô�����*/
    if (setjmp(jerr.setjmp_buffer))
    {
        jpeg_destroy_decompress(&cinfo);
        return FALSE;
    }

    // �����ڴ�ͼ��
    jpeg_create_decompress(&cinfo);
    jpeg_mem_src(&cinfo, (const unsigned char*)Data, _msize((void*)Data));
    // ��ȡͼ����Ϣ
    jpeg_read_header(&cinfo, TRUE);
    cinfo.out_color_space = JCS_RGB;
    // ������Ϣ
    image->Width = cinfo.image_width;
    image->Hight = cinfo.image_height;
    image->Depth = cinfo.num_components;
    // ����ռ�
    image->Pixel = (void*)malloc(cinfo.image_width * cinfo.image_height * cinfo.num_components);

    // ��ȡͼ��
    jpeg_start_decompress(&cinfo);
    JSAMPROW buffer;
    while (cinfo.output_scanline < cinfo.output_height)
    {
        buffer = (JSAMPROW) & ((char*)image->Pixel)[(cinfo.output_height - cinfo.output_scanline - 1) * cinfo.image_width * cinfo.num_components];
        jpeg_read_scanlines(&cinfo,
            (JSAMPARRAY)&buffer,
            1);
    }
    
    // �������
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    
    return TRUE;
}

/**
 * ����jpeg
 *
 * \param ** Data   : Ҫ�����ָ��
 * \param *image    : ͼ���ָ��
 * \param quality   : ѹ����(0-100)0Ϊ���
 *
 * \returns ���ݴ�С,������0���ȡ����
 */
unsigned int Jpeg_Save(void** Data, const struct IMAGE* image, int quality)
{
    struct jpeg_compress_struct cinfo;
    struct Jpeg_Error jem;
    unsigned int Size = 0;

    /*���ô�����*/
    cinfo.err = jpeg_std_error(&jem.pub);
    jem.pub.error_exit = my_error_exit;

    /*���ô�����*/
    if (setjmp(jem.setjmp_buffer))
    {
        free(*Data);
        *Data = NULL;
        jpeg_destroy_compress(&cinfo);
        return 0;
    }

    // ����ѹ������
    jpeg_create_compress(&cinfo);
    jpeg_mem_dest(&cinfo, (unsigned char**)Data, &Size);
    // ����ѹ������
    cinfo.image_width = image->Width;
    cinfo.image_height = image->Hight;
    cinfo.input_components = image->Depth;
    cinfo.in_color_space = JCS_RGB;
    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, quality, TRUE);
    // ��ʼѹ��
    jpeg_start_compress(&cinfo, TRUE);

    JSAMPROW buffer;

    // ��ÿһ�н���ѹ��
    while (cinfo.next_scanline < cinfo.image_height) {
        
        buffer = &(((char*)image->Pixel)[(cinfo.image_height - cinfo.next_scanline - 1) * cinfo.image_width * cinfo.num_components]);
        jpeg_write_scanlines(&cinfo, (JSAMPARRAY)&buffer, 1);
    }

    // �������
    jpeg_finish_compress(&cinfo);
    jpeg_destroy_compress(&cinfo);

    return Size;
}

// ������
inline METHODDEF(void) my_error_exit(j_common_ptr cinfo)
{
#ifdef _DEBUG
    printf("jpeg��ʽ���ִ���!\n");
#endif
    longjmp(((struct Jpeg_Error*)cinfo->err)->setjmp_buffer, 1);
}