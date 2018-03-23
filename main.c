#include <stdio.h>
#define PIXEL_SIZE 512

typedef struct _pixel {
    unsigned short int red;
    unsigned short int green;
    unsigned short int blue;
} Pixel;

typedef struct _image {
    Pixel pixel[PIXEL_SIZE][PIXEL_SIZE];
    unsigned int width;
    unsigned int height;

} Image;

int max(int a, int b) {
    if (a > b)
        return a;
    return b;
}

Image grayScale(Image img) {

    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            int media = img.pixel[i][j].red +
                        img.pixel[i][j].green +
                        img.pixel[i][j].blue;
            media /= 3;
            img.pixel[i][j].red = media;
            img.pixel[i][j].green = media;
            img.pixel[i][j].blue = media;
        }
    }

    return img;
}


Image blur(Image img, int T) {
    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            Pixel media = {0, 0, 0};

            int min_h = (img.height - 1 > i + T/2) ? i + T/2 : img.height - 1;
            int min_w = (img.width - 1 > j + T/2) ? j + T/2 : img.width - 1;

            int x = max(0,i - T/2);
            int y = max(0, j - T/2);

            for(x ; x <= min_h; ++x) {
                for(int y = (0 > j - T/2 ? 0 : j - T/2); y <= min_w; ++y) {
                    media.red += img.pixel[x][y].red;
                    media.green += img.pixel[x][y].green;
                    media.blue += img.pixel[x][y].blue;
                }
            }

            media.red /= T * T;
            media.green /= T * T;
            media.blue /= T * T;

            img.pixel[i][j].red = media.red;
            img.pixel[i][j].green = media.green;
            img.pixel[i][j].blue = media.blue;
        }
    }
    return img;
}


Image rotatePicture(Image img) {
    Image rotateded;

    rotateded.width = img.height;
    rotateded.height = img.width;

    for (unsigned int i = 0, y = 0; i < rotateded.height; ++i, ++y) {
        for (int j = rotateded.width - 1, x = 0; j >= 0; --j, ++x) {
            rotateded.pixel[i][j].red = img.pixel[x][y].red;
            rotateded.pixel[i][j].green = img.pixel[x][y].green;
            rotateded.pixel[i][j].blue = img.pixel[x][y].blue;
        }
    }

    return rotateded;
}


Image reverseColors(Image img) {
    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            img.pixel[i][j].red = 255 - img.pixel[i][j].red;
            img.pixel[i][j].green = 255 - img.pixel[i][j].green;
            img.pixel[i][j].blue = 255 - img.pixel[i][j].blue;
        }
    }
    return img;
}


Image cropImage(Image img, int x, int y, int  newWidth, int newHeight) {
    Image croppedImage;

    croppedImage.width = newWidth;
    croppedImage.height = newHeight;

    for(int i = 0; i < newHeight; ++i) {
        for(int j = 0; j < newWidth; ++j) {
            croppedImage.pixel[i][j].red = img.pixel[i + y][j + x].red;
            croppedImage.pixel[i][j].green = img.pixel[i + y][j + x].green;
            croppedImage.pixel[i][j].blue = img.pixel[i + y][j + x].blue;
        }
    }

    return croppedImage;
}

Image readPixel(Image img){

  for (unsigned int i = 0; i < img.height; ++i) {
      for (unsigned int j = 0; j < img.width; ++j) {
            scanf("%hu %hu %hu", &img.pixel[i][j].red,
                               &img.pixel[i][j].green,
                               &img.pixel[i][j].blue);

      }
  }
  return img;
}

Image sepiaFilter(Image img){
  for (unsigned int x = 0; x < img.height; ++x) {
      for (unsigned int j = 0; j < img.width; ++j) {
          unsigned short int pixel[3];
          pixel[0] = img.pixel[x][j].red;
          pixel[1] = img.pixel[x][j].green;
          pixel[2] = img.pixel[x][j].blue;

          int p =  pixel[0] * .393 + pixel[1] * .769 + pixel[2] * .189;
          int menor_r = (255 >  p) ? p : 255;
          img.pixel[x][j].red = menor_r;

          p =  pixel[0] * .349 + pixel[1] * .686 + pixel[2] * .168;
          menor_r = (255 >  p) ? p : 255;
          img.pixel[x][j].green = menor_r;

          p =  pixel[0] * .272 + pixel[1] * .534 + pixel[2] * .131;
          menor_r = (255 >  p) ? p : 255;
          img.pixel[x][j].blue = menor_r;
      }
  }

  return img;
}

Image mirroringFilter(Image img){
  int horizontal = 0;
  scanf("%d", &horizontal);

  int width = img.width, height = img.height;

  if (horizontal == 1){

    width /= 2;
  }
  else{

    height /= 2;
  }

  for (int i2 = 0; i2 < height; ++i2) {
      for (int j = 0; j < width; ++j) {
          int x = i2, y = j;

          if (horizontal == 1){
            y = img.width - 1 - j;
          }
          else{
            x = img.height - 1 - i2;
          }

          Pixel aux1;
          aux1.red = img.pixel[i2][j].red;
          aux1.green = img.pixel[i2][j].green;
          aux1.blue = img.pixel[i2][j].blue;

          img.pixel[i2][j].red = img.pixel[x][y].red;
          img.pixel[i2][j].green = img.pixel[x][y].green;
          img.pixel[i2][j].blue = img.pixel[x][y].blue;

          img.pixel[x][y].red = aux1.red;
          img.pixel[x][y].green = aux1.green;
          img.pixel[x][y].blue = aux1.blue;
      }
  }


  return img;
}

Image printPixels(Image img){
  for (unsigned int i = 0; i < img.height; ++i) {
      for (unsigned int j = 0; j < img.width; ++j) {
          printf("%hu %hu %hu ", img.pixel[i][j].red,
                                 img.pixel[i][j].green,
                                 img.pixel[i][j].blue);

      }
  }
  printf("\n");
  return img;
}

int main() {
    Image img;

    // read type of image
    char p3[4];
    scanf("%s", p3);

    // read width height and color of image
    int max_color;
    scanf("%u %u %d", &img.width, &img.height, &max_color);

    // read all pixels of image

    img = readPixel(img);

    int n_opcoes;
    scanf("%d", &n_opcoes);

    for(int i = 0; i < n_opcoes; ++i) {
        int opcao;
        scanf("%d", &opcao);

        switch(opcao) {
            case 1: { // Escala de Cinza
                img = grayScale(img);
                break;
            }
            case 2: { // Filtro Sepia
                img = sepiaFilter(img);

                break;
            }
            case 3: { // Blur

                int tamanho = 0;
                scanf("%d", &tamanho);
                img = blur(img,tamanho);
                break;

            }
            case 4: { // Rotacao

                int quantas_vezes = 0;
                scanf("%d", &quantas_vezes);
                quantas_vezes %= 4;
                for (int j = 0; j < quantas_vezes; ++j) {
                    img = rotatePicture(img);
                }
                break;

            }
            case 5: { // Espelhamento
                img = mirroringFilter(img);
                break;
            }
            case 6: { // Inversao de Cores
                img  = reverseColors(img);
                break;
            }
            case 7: { // Cortar Imagem

                int x, y;
                scanf("%d %d", &x, &y);
                int width, height;
                scanf("%d %d", &width, &height);

                img = cropImage(img, x, y, width, height);

                break;

            }
        }

    }

    // print type of image
    printf("P3\n");
    // print width height and color of image
    printf("%u %u\n255\n", img.width, img.height);
    img = printPixels(img);

    return 0;
}
