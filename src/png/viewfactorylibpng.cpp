#include <png.h>

#include "viewfactorylibpng.h"
#include <gl/gltexture.h>

/// @param resPath Path to resource dir
ViewFactoryLibPNG::ViewFactoryLibPNG(std::string resPath):
    resPath(resPath){

}

// http://www.libpng.org/pub/png/book/chapter13.html
/// @param path Relative path to image in resource dir without extension.
/// An image has to be saved with alpha chanel.
std::shared_ptr<GLTexture> ViewFactoryLibPNG::getGLTexture(std::string path) const {
    std::string fullPath = resPath + "/" + path + ".png";
    FILE *fp = fopen(fullPath.data(), "rb");
    if (!fp)
        throw file_open_error();

    png_byte sig[8];
    fread(sig, 1, 8, fp);
    if (!png_check_sig(sig, 8)) {
        fclose(fp);
        throw wrong_file_format();
    }

    static png_structp png_ptr;
    static png_infop info_ptr;

    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        fclose(fp);
        throw out_of_memory();
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        fclose(fp);
        throw out_of_memory();
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        throw io_error();
    }

    png_init_io(png_ptr, fp);
    png_set_sig_bytes(png_ptr, 8);
    png_read_info(png_ptr, info_ptr);

    int width = png_get_image_width(png_ptr, info_ptr);
    int height = png_get_image_height(png_ptr, info_ptr);

    //TODO: I can fix bitmap format after reading
    if (png_get_rowbytes(png_ptr,info_ptr) / width != 4) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        throw wrong_bitmap_format();
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        throw read_error();
    }

    png_bytep* row_pointers = new png_bytep[sizeof(png_bytep) * height];
    png_byte* img_data = new png_byte[png_get_rowbytes(png_ptr,info_ptr) * height];
    for (int y = 0; y < height; y++)
        row_pointers[y] = img_data + png_get_rowbytes(png_ptr,info_ptr) * y;

    png_read_image(png_ptr, row_pointers);

    //TODO: Use PNG_TRANSFORM_BGR instead maual RGB switching
    for (int i = 0; i < height*png_get_rowbytes(png_ptr,info_ptr) / 4; i++) {
        png_byte c = img_data[i * 4];
        img_data[i * 4] = img_data[i * 4 + 2];
        img_data[i * 4 + 2] = c;
    }

    auto result = std::make_shared<GLTexture>((char *)img_data, width, height);

    delete [] row_pointers;
    delete [] img_data;

    //TODO: Is it a proper cleanup?
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(fp);
    return result;
}
