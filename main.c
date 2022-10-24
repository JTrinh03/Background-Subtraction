#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#define STB_IMAGE_IMPLEMENTATION
#include "./headers/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "./headers/stb_image_write.h"

unsigned char *newarray_1d(int size)
{
    return (unsigned char *)calloc(size, sizeof(unsigned char));
}

unsigned char *thediff(unsigned char *background, unsigned char *foreground, int width, int height, int channel)
{
    unsigned char *diffimage= newarray_1d(height*width);
    for(int i=0; i< height; i++)
    {
        for(int j=0; j<width; j++)
        {
            int sum= 0;
            for(int k=0; k< channel; k++)
            {
                sum+= abs(background[i*width*channel+k]-foreground[i*width*channel+j*channel+k]);

            }
            if((sum/3)<55)
            {
                diffimage[i*width+j]=0;
            }
            else
            {
                diffimage[i*width+j]=255;
            }
        }
    }
    return diffimage;
}

unsigned char *imageadding(unsigned char *diffimage, unsigned char *new_background, unsigned char *foreground, int width, int height, int channel)
{
    for(int i=0; i<height; i++)
    {
        for(int j=0; j< width; j++)
        {
            if(diffimage[i*width+j]==255)
               {
                   for(int k=0; k<channel; k++)
                   {
                       new_background[i*width*channel +j*channel +k]= foreground[i*width*channel +j*channel +k];

                   }

               }
        }
    }
}

int main()
{
    int width;
    int height;
    int channel;
    char background_location[]= "./images/background.jpg";
    char foreground_location[]="./images/foreground.jpg";
    char difference_location[]= "./images/difference.jpg";
    char binarydifference_location[]="./images/binary_difference.jpg";
    unsigned char *background= stbi_load(background_location, &width, &height, &channel, 0);
    unsigned char *foreground= stbi_load(foreground_location, &width, &height, &channel, 0);

    if(background == NULL || foreground == NULL)
    {
        printf(" Error1!!!\n");
        exit(1);
    }
    unsigned char *diffimage= thediff(background, foreground, width, height, channel);
    stbi_write_png(binarydifference_location, width, height,1,diffimage,width);
    printf("The new image is saved at %s\n", binarydifference_location);

    char newbackground_location[]= "./images/new_background.jpg";
    char output_location[]= "./images/output.png";
    unsigned char *new_background= stbi_load(newbackground_location, &width, &height, &channel, 0);

    if( new_background == NULL)
    {
        printf(" Error2!!!\n");
        exit(1);
    }
    imageadding(diffimage, new_background, foreground, width, height, channel);
    stbi_write_png(output_location, width, height, channel, new_background, width*channel);
    printf("The new image is saved at %s\n", output_location);
}
