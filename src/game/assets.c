// Copyright (C) 2012 Mikel Izal Azcarate
// Copyright (C) 2012 Bware Studios
//
// This software is distributed under a MIT license, see LICENSE file for details

#include "pilgrim.h"


// hace falta opengl y core graphics o equivalentes para crear las texturas
#if TARGET_OS_IPHONE
#include <CoreGraphics/CoreGraphics.h>
#else
#include <ApplicationServices/ApplicationServices.h>
#endif
#if TARGET_OS_IPHONE
#import <QuartzCore/QuartzCore.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#else
#import <OpenGL/gl.h>
#endif

#import <AudioToolbox/AudioToolbox.h>

#import <OpenAL/al.h>
#import <OpenAL/alc.h>

int assets_texture_max_wh=2048;




void assets_getaudiofile(const char *filename,ALuint sndbuffer);

const char *assets_resource_path = NULL;


t_assets_image_infoPtr assets_images=NULL;

t_assets_tile_infoPtr assets_tiles=NULL;

t_assets_tile_infoPtr assets_labels=NULL;


t_bool assets_load(const char *filename) {
    // load all assets described in file
    //   la idea es de momento que todo este en un fichero pero en el futuro poder elegir varios y cargar uno
    //   y liberar todos los de un fichero
    // 
    // tex num nombrefichero
    // img num  x y w h         off_x off_y                        col_w col_h col_off_x col_off_y  # description
    //           rect bounds      posicion del centro en el rect    col rect respecto al centro
    // img num  parametros
    //
    // snd num filename       # un clip de sonido  habra que pensar como identificar los equivalentes
    // music num filenam      # una frase musical
    // ^---- solo significativa la primera letra para facilitar el parseo
    
    char fullfilename[256];
    char linea[256];
    FILE *f;
    
    sprintf(fullfilename,"%s/%s",assets_resource_path,filename);
    
    f=fopen(fullfilename,"r");
    if (f==NULL) {
        printf("Error cannot open file %s\n",filename);
        exit(0);
    }
    int count_imgs=0;
    int count_snds=0;
    int count_musics=0;
    int count_tiles=0;
    int count_labels=0;
    while ( fgets(linea,255,f) ) {
        switch (linea[0]) {
            case 'i':
                count_imgs+=1;
                break;
            case 's':
                count_snds+=1;
                break;
            case 'm':
                count_musics+=1;
                break;
            case 'f':
                count_tiles+=1;
                break;
            case 'l':
                count_labels+=1;
                break;
            default:
                break;
        }
    }
    rewind(f);
    assets_images=malloc(sizeof(t_assets_image_info)*count_imgs);
    assets_tiles=malloc(sizeof(t_assets_tile_info)*count_tiles);
    assets_labels=malloc(sizeof(t_assets_tile_info)*count_labels);
    
    audio_init(count_snds,count_musics);

    int i_tex_num=0,i_img_num=0;
    int i_snd_num=0,i_mus_num=0;
    int i_tile_num=0, i_label_num=0;
    int tex_id;
    int tex_container_w,tex_container_h;
    while ( fgets(linea,255,f) ) {
        if (linea[0]!='#') {
            if (linea[0]=='t') {
                int tex_num;
                char texfilename[100];
                sscanf(linea,"tex %u %s",&tex_num,texfilename);
                tex_id=assets_gettexture(texfilename,&tex_container_w,&tex_container_h);
                if (tex_num!=i_tex_num) {
                    printf("error de numero de textura\n");
                    exit(0);
                }
//                printf("tex %u loaded\n",i_tex_num);
                i_tex_num+=1;
            } else if (linea[0]=='i') {
                int img_num;
                int nread;
                t_pos i_x,i_y,i_w,i_h,i_ox,i_oy,i_colw,i_colh,i_colox,i_coloy;
                nread=sscanf(linea,"img %u %f %f %f %f %f %f %f %f %f %f",&img_num,&i_x,&i_y,&i_w,&i_h,&i_ox,&i_oy,&i_colw,&i_colh,&i_colox,&i_coloy);
                if (nread!=11) {
                    printf("error leyendo parametros de imagen\n");
                    exit(0);
                }
                if (img_num!=i_img_num) {
                    printf("error de numero de imagen\n");
                    exit(0);
                }
                assets_images[img_num].tex_id=tex_id;
                assets_images[img_num].tex_container_w=tex_container_w;
                assets_images[img_num].tex_container_h=tex_container_h;
                assets_images[img_num].r_x=i_x;
                assets_images[img_num].r_y=i_y;
                assets_images[img_num].r_w=i_w;
                assets_images[img_num].r_h=i_h;
                assets_images[img_num].off_x=i_ox;
                assets_images[img_num].off_y=i_oy;
                assets_images[img_num].col_w=i_colw;
                assets_images[img_num].col_h=i_colh;
                assets_images[img_num].col_off_x=i_colox;
                assets_images[img_num].col_off_y=i_coloy;
//                printf("img %u loaded\n",i_img_num);
                i_img_num+=1;
            } else if (linea[0]=='f') {
                int tile_num;
                int nread;
                t_pos i_x,i_y,i_w,i_h;
                nread=sscanf(linea,"fnd %u %f %f %f %f",&tile_num,&i_x,&i_y,&i_w,&i_h);
                if (nread!=5) {
                    printf("error leyendo parametros de tile\n");
                    exit(0);                   
                }
                if (tile_num!=i_tile_num) {
                    printf("error de numero de tile %d expected %d\n",tile_num,i_tile_num);
                    exit(0);
                }
                assets_tiles[tile_num].tex_id=tex_id;
                assets_tiles[tile_num].out_bounds.w=i_w;
                assets_tiles[tile_num].out_bounds.h=i_h;
                assets_tiles[tile_num].out_bounds.off_x=0;
                assets_tiles[tile_num].out_bounds.off_y=0;
                generate_vertex_data_from_bb(assets_tiles[tile_num].vertex_data, &(assets_tiles[tile_num].out_bounds));
                generate_texcoord_data_from_rect(assets_tiles[tile_num].tex_data, i_x, i_y, i_w, i_h, tex_container_w, tex_container_h);
//                printf("fnd %u loaded\n",i_tile_num);
                i_tile_num+=1;
            } else if (linea[0]=='l') {
                int label_num;
                int nread;
                t_pos i_x,i_y,i_w,i_h;
                nread=sscanf(linea,"lbl %u %f %f %f %f",&label_num,&i_x,&i_y,&i_w,&i_h);
                if (nread!=5) {
                    printf("error leyendo parametros de label\n");
                    exit(0);                   
                }
                if (label_num!=i_label_num) {
                    printf("error de numero de label %d expected %d\n",label_num,i_label_num);
                    exit(0);
                }
                assets_labels[label_num].tex_id=tex_id;
                assets_labels[label_num].out_bounds.w=i_w;
                assets_labels[label_num].out_bounds.h=i_h;
                assets_labels[label_num].out_bounds.off_x=0;
                assets_labels[label_num].out_bounds.off_y=0;
                generate_vertex_data_from_bb(assets_labels[label_num].vertex_data, &(assets_labels[label_num].out_bounds));
                generate_texcoord_data_from_rect(assets_labels[label_num].tex_data, i_x, i_y, i_w, i_h, tex_container_w, tex_container_h);
//                printf("lbl %u loaded\n",i_label_num);
                i_label_num+=1;
            } else if (linea[0]=='s') {
                // cargar sonido
                int snd_num;
                char sndfilename[100];
                sscanf(linea,"snd %u %s",&snd_num,sndfilename);
                if (snd_num!=i_snd_num) {
                    printf("error de numero de sonido\n");
                    exit(0);
                }
                assets_getaudiofile(sndfilename,audio_snd_buffer[snd_num]);
                i_snd_num+=1;
            } else if (linea[0]=='m') {
                // cargar musica
                int mus_num;
                char musfilename[100];
                sscanf(linea,"mus %u %s",&mus_num,musfilename);
                if (mus_num!=i_mus_num) {
                    printf("error de numero de musica\n");
                    exit(0);
                }
                assets_getaudiofile(musfilename,audio_mus_buffer[mus_num]);
                i_mus_num+=1;
            }
        }
    }
    fclose(f);
    
    sprite_classes_init();
    item_classes_init();
//    sprite_test_print_sprite_class_tree();

    text_init();
    text_credits_init();
    
    free(assets_images);
    assets_images=NULL;
    
    return true;
}

    

void assets_setresourcepath(const char *resourcepath) {
    assets_resource_path=resourcepath;
//    printf("assets resourcepath set to: %s\n",assets_resource_path);
}





int assets_gettexture(const char *filename,int *containerw,int *containerh) {
    // Loads a texture from filename
    // loads to openGL and dispose any resources used
    // returns openGL texid
    
    
    // de momento solo carga png y similares
    // hacer que implemente tambien carga de texturas PVRTC que pueden pasarsele directamente a openglES 
    
    CGDataProviderRef fileprovider=NULL;
    //    CGImageSourceRef imagesource=NULL;
    CGImageRef iref=NULL;
    void *imagebuf;
    char fullfilename[256];
    
    int buf_w,buf_h;
    int img_w,img_h;
    
    sprintf(fullfilename,"%s/%s",assets_resource_path,filename);
    //printf("ffn: %lx fileprovantes: %lx \n",fullfilename,fileprovider);
    fileprovider=CGDataProviderCreateWithFilename(fullfilename);
    //printf("fileprov: %lx \n",fileprovider);
    if (fileprovider==NULL) {
        printf("Error cargando textura del fichero %s %s\n",filename,fullfilename);
        return -1;
    }
    // // Esto del imagesource esta bien pero no funciona en el iphone
    //    imagesource=CGImageSourceCreateWithDataProvider(fileprovider, NULL);
    //    if (imagesource==NULL) {
    //        printf("Error cargando textura del fichero %s %s formato incorrecto\n",filename,fullfilename);
    //        CGDataProviderRelease(fileprovider);
    //        return -1;
    //    }
    //    iref=CGImageSourceCreateImageAtIndex(imagesource, 0, NULL);
    iref=CGImageCreateWithPNGDataProvider(fileprovider, NULL, FALSE, kCGRenderingIntentDefault);
    if (iref==NULL) {
        printf("Error cargando textura del fichero %s %s no puedo crear la imagen\n",filename,fullfilename);
        //        CFRelease(imagesource);
        CGDataProviderRelease(fileprovider);
        return -1;
    }
    //printf("img: %x\n",iref);
    img_w=(int)CGImageGetWidth(iref);
    img_h=(int)CGImageGetHeight(iref);
//    printf("w: %u h: %u\n",img_w,img_h);
    buf_w=(int)pow(2,(1+(int)(log(img_w-1)/log(2))));
    buf_h=(int)pow(2,(1+(int)(log(img_h-1)/log(2))));
    if (buf_w>assets_texture_max_wh) buf_w=assets_texture_max_wh;
    if (buf_h>assets_texture_max_wh) buf_h=assets_texture_max_wh;
//    printf("w: %u h: %u\n",buf_w,buf_h);
    imagebuf=calloc(buf_w*buf_h,4);
    //printf("imagebuf: %lx \n",imagebuf);
    //    CGColorRef green;
    CGColorSpaceRef rgbcolorspace=CGColorSpaceCreateDeviceRGB();
    CGContextRef bufCtx=CGBitmapContextCreate(imagebuf, buf_w, buf_h, 8, 4*buf_w, rgbcolorspace, kCGImageAlphaPremultipliedLast);
    //    // parece que esta funcion no esta en el iphone y hay que crear el colorspace
    //    // en mac os x valia con esta funcion
    //    //green=CGColorCreateGenericRGB(0, 1, 0, 1);
    //    CGFloat components[4]={0.0,1.0,0.0,1.0};
    //    green=CGColorCreate(rgbcolorspace,components);
    //    CGContextSetFillColorWithColor(bufCtx, green);    
    //    CGColorRelease(green);
    CGColorSpaceRelease(rgbcolorspace);
    
    // para poder llenar el fondo de la textura si quiero quitar la transparencia
    //CGContextFillRect(bufCtx,CGRectMake(0, 0 , buf_w, buf_h));
    
    CGContextDrawImage(bufCtx, CGRectMake(0, 0 , img_w, img_h), iref);
    CGContextRelease(bufCtx);
    CGImageRelease(iref);
    //    CFRelease(imagesource);
    CGDataProviderRelease(fileprovider);
    
//    printf("texture to opengl...\n");
    
    GLuint tex_name;
    
    glGenTextures(1, &tex_name);
    glBindTexture(GL_TEXTURE_2D, tex_name);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, buf_w, buf_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, imagebuf);
    GLuint err=glGetError();
    if (err != GL_NO_ERROR) {
        printf("glTexImage2D(): Error al cargar la textura e=%u\n",err);
        free(imagebuf);
        return -1;
    }

    if (containerh!=NULL) *containerh=buf_h;
    if (containerw!=NULL) *containerw=buf_w;
    
    free(imagebuf);
    return tex_name;
}

//void assets_getaudiofile(const char *filename,ALuint sndbuffer) {
void assets_getaudiofile(const char *filename,unsigned int sndbuffer) {
    
    char fullfilename[256];
    AudioFileID fid;
    OSStatus err;
        
//    printf("loading sound file %s\n",filename);
    sprintf(fullfilename,"%s/%s",assets_resource_path,filename);
    CFURLRef fileurl=CFURLCreateFromFileSystemRepresentation(NULL, (UInt8*)fullfilename, strlen(fullfilename), false);
    err=AudioFileOpenURL(fileurl, kAudioFileReadPermission, 0, &fid);
    //printf("fid= %x err= %d\n",fid,err);

    UInt64 audiosize=0;
    UInt32 valuesize=sizeof(UInt64);
    err=AudioFileGetProperty(fid, kAudioFilePropertyAudioDataByteCount, &valuesize, &audiosize);
    //printf("err=%x audiosize=%d \n",err,audiosize);
    
    
    AudioStreamBasicDescription dataformat;
    valuesize=sizeof(dataformat);
    err=AudioFileGetProperty(fid, kAudioFilePropertyDataFormat, &valuesize, &dataformat);
    //printf("err=%x channels: %d samplerate: %f\n",err,dataformat.mChannelsPerFrame,dataformat.mSampleRate);
//    valuesize=sizeof(fformat);
//    err=AudioFileGetProperty(fid, kAudioFilePropertyFileFormat, &valuesize, fformat);
//    printf("err=%x file:%s \n",err,fformat);

    UInt64 duration;
    valuesize=sizeof(duration);
    err=AudioFileGetProperty(fid, kAudioFilePropertyEstimatedDuration, &valuesize, &duration);
    //printf("err=%x duration:%f \n",err,duration);

    ALenum formato;
    t_bool stereo=dataformat.mChannelsPerFrame==2;
    t_bool mono=dataformat.mChannelsPerFrame==1;
    int bits_per_channel=dataformat.mBitsPerChannel;
    
//    printf("%d bits per channel %s%s\n",bits_per_channel,mono?"mono":"",stereo?"stereo":"");
    
    if (mono) {
        if (bits_per_channel==8)
            formato=AL_FORMAT_MONO8;    // esto espero 8 bits unsigned int por canal
        else if (bits_per_channel==16)
            formato=AL_FORMAT_MONO16;    // esto espero 16 bits signed int por canal
        else {
            printf("audio error formato extraño en el fichero %s bitsperchannel=%d\n",filename,bits_per_channel);
            exit(0);
        }
    } else if (stereo) {
        printf("warning: audiofile stereo seguro que quieres??");
        if (bits_per_channel==8)
            formato=AL_FORMAT_STEREO8;
        else if (bits_per_channel==16)
            formato=AL_FORMAT_STEREO16;
        else {
            printf("audio error formato extraño en el fichero %s stereo y bitsperchannel=%d\n",filename,bits_per_channel);
            exit(0);
        }
    } else {
        printf("audio error formato extraño en el fichero %s hay %d canales\n",filename,(int)dataformat.mChannelsPerFrame);
        exit(0);
    }
    
    unsigned char *sound_data_buffer=malloc(audiosize);
    
    
    err=AudioFileReadBytes(fid, false, 0, (UInt32*)&audiosize, sound_data_buffer);
    //printf("err=%x audiosize=%d \n",err,audiosize);

    
    err=alGetError();
    alBufferData(sndbuffer, formato, sound_data_buffer, (UInt32)audiosize,(int)dataformat.mSampleRate);
//    printf("buf: %d\n",sndbuffer);
//    audio_print_openalerrorstate_at("audio load buffer: ");
    
    AudioFileClose(fid);
    
    free(sound_data_buffer);
}


