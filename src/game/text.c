// Copyright (C) 2012 Mikel Izal Azcarate
// Copyright (C) 2012 Bware Studios
//
// This software is distributed under a MIT license, see LICENSE file for details

#include "pilgrim.h"

#if TARGET_OS_IPHONE
#include <CoreGraphics/CoreGraphics.h>
#include <CoreFoundation/CoreFoundation.h>
#include <CoreText/CoreText.h>
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


#include <string.h>


t_assets_tile_info text_credits_tile;
t_assets_tile_info text_highscores_tile;

t_assets_tile_info text_menu_start_tile;
t_assets_tile_info text_menu_start_selected_tile;
t_assets_tile_info text_menu_help_tile;
t_assets_tile_info text_menu_help_selected_tile;
t_assets_tile_info text_menu_credits_tile;
t_assets_tile_info text_menu_credits_selected_tile;
t_assets_tile_info text_menu_hof_tile;
t_assets_tile_info text_menu_hof_selected_tile;

t_assets_tile_info text_gameover_tile;


void text_init(void) {
    text_highscores_tile.tex_id=-1;
    text_credits_tile.tex_id=-1;
}


void text_credits_init(void) {
    int tex_container_w=1024;
    int tex_container_h=1024;
    
    text_credits_tile.tex_id=text_credits_gettexture(tex_container_w,tex_container_h);
    text_credits_tile.out_bounds.w=900.0;
    text_credits_tile.out_bounds.h=600.0;
    text_credits_tile.out_bounds.off_x=0.0;
    text_credits_tile.out_bounds.off_y=0.0;
    generate_vertex_data_from_bb(text_credits_tile.vertex_data, &text_credits_tile.out_bounds);
    generate_texcoord_data_from_rect(text_credits_tile.tex_data, 0.0, 0.0, text_credits_tile.out_bounds.w, text_credits_tile.out_bounds.h, tex_container_w, tex_container_h);


    text_menu_start_tile.tex_id=text_credits_tile.tex_id;
    text_menu_start_tile.out_bounds.w=250.0;
    text_menu_start_tile.out_bounds.h=45.0;
    text_menu_start_tile.out_bounds.off_x=0.0;
    text_menu_start_tile.out_bounds.off_y=0.0;
    generate_vertex_data_from_bb(text_menu_start_tile.vertex_data, &text_menu_start_tile.out_bounds);
    generate_texcoord_data_from_rect(text_menu_start_tile.tex_data, 0.0, 700.0, text_menu_start_tile.out_bounds.w, text_menu_start_tile.out_bounds.h, tex_container_w, tex_container_h);

    
    text_menu_start_selected_tile.tex_id=text_credits_tile.tex_id;
    text_menu_start_selected_tile.out_bounds.w=250.0;
    text_menu_start_selected_tile.out_bounds.h=45.0;
    text_menu_start_selected_tile.out_bounds.off_x=0.0;
    text_menu_start_selected_tile.out_bounds.off_y=0.0;
    generate_vertex_data_from_bb(text_menu_start_selected_tile.vertex_data, &text_menu_start_selected_tile.out_bounds);
    generate_texcoord_data_from_rect(text_menu_start_selected_tile.tex_data, 250.0, 700.0, text_menu_start_selected_tile.out_bounds.w, text_menu_start_selected_tile.out_bounds.h, tex_container_w, tex_container_h);
    

    text_menu_help_tile.tex_id=text_credits_tile.tex_id;
    text_menu_help_tile.out_bounds.w=250.0;
    text_menu_help_tile.out_bounds.h=45.0;
    text_menu_help_tile.out_bounds.off_x=0.0;
    text_menu_help_tile.out_bounds.off_y=0.0;
    generate_vertex_data_from_bb(text_menu_help_tile.vertex_data, &text_menu_help_tile.out_bounds);
    generate_texcoord_data_from_rect(text_menu_help_tile.tex_data, 0.0, 750.0, text_menu_help_tile.out_bounds.w, text_menu_help_tile.out_bounds.h, tex_container_w, tex_container_h);
    
    
    text_menu_help_selected_tile.tex_id=text_credits_tile.tex_id;
    text_menu_help_selected_tile.out_bounds.w=250.0;
    text_menu_help_selected_tile.out_bounds.h=45.0;
    text_menu_help_selected_tile.out_bounds.off_x=0.0;
    text_menu_help_selected_tile.out_bounds.off_y=0.0;
    generate_vertex_data_from_bb(text_menu_help_selected_tile.vertex_data, &text_menu_help_selected_tile.out_bounds);
    generate_texcoord_data_from_rect(text_menu_help_selected_tile.tex_data, 250.0, 750.0, text_menu_help_selected_tile.out_bounds.w, text_menu_help_selected_tile.out_bounds.h, tex_container_w, tex_container_h);

    text_menu_credits_tile.tex_id=text_credits_tile.tex_id;
    text_menu_credits_tile.out_bounds.w=250.0;
    text_menu_credits_tile.out_bounds.h=45.0;
    text_menu_credits_tile.out_bounds.off_x=0.0;
    text_menu_credits_tile.out_bounds.off_y=0.0;
    generate_vertex_data_from_bb(text_menu_credits_tile.vertex_data, &text_menu_credits_tile.out_bounds);
    generate_texcoord_data_from_rect(text_menu_credits_tile.tex_data, 0.0, 800.0, text_menu_credits_tile.out_bounds.w, text_menu_credits_tile.out_bounds.h, tex_container_w, tex_container_h);
    
    
    text_menu_credits_selected_tile.tex_id=text_credits_tile.tex_id;
    text_menu_credits_selected_tile.out_bounds.w=250.0;
    text_menu_credits_selected_tile.out_bounds.h=45.0;
    text_menu_credits_selected_tile.out_bounds.off_x=0.0;
    text_menu_credits_selected_tile.out_bounds.off_y=0.0;
    generate_vertex_data_from_bb(text_menu_credits_selected_tile.vertex_data, &text_menu_credits_selected_tile.out_bounds);
    generate_texcoord_data_from_rect(text_menu_credits_selected_tile.tex_data, 250.0, 800.0, text_menu_credits_selected_tile.out_bounds.w, text_menu_credits_selected_tile.out_bounds.h, tex_container_w, tex_container_h);
   
    text_menu_hof_tile.tex_id=text_credits_tile.tex_id;
    text_menu_hof_tile.out_bounds.w=250.0;
    text_menu_hof_tile.out_bounds.h=45.0;
    text_menu_hof_tile.out_bounds.off_x=0.0;
    text_menu_hof_tile.out_bounds.off_y=0.0;
    generate_vertex_data_from_bb(text_menu_hof_tile.vertex_data, &text_menu_hof_tile.out_bounds);
    generate_texcoord_data_from_rect(text_menu_hof_tile.tex_data, 0.0, 850.0, text_menu_hof_tile.out_bounds.w, text_menu_hof_tile.out_bounds.h, tex_container_w, tex_container_h);
    
    
    text_menu_hof_selected_tile.tex_id=text_credits_tile.tex_id;
    text_menu_hof_selected_tile.out_bounds.w=250.0;
    text_menu_hof_selected_tile.out_bounds.h=45.0;
    text_menu_hof_selected_tile.out_bounds.off_x=0.0;
    text_menu_hof_selected_tile.out_bounds.off_y=0.0;
    generate_vertex_data_from_bb(text_menu_hof_selected_tile.vertex_data, &text_menu_hof_selected_tile.out_bounds);
    generate_texcoord_data_from_rect(text_menu_hof_selected_tile.tex_data, 250.0, 850.0, text_menu_hof_selected_tile.out_bounds.w, text_menu_hof_selected_tile.out_bounds.h, tex_container_w, tex_container_h);
    
    
    text_gameover_tile.tex_id=text_credits_tile.tex_id;
    text_gameover_tile.out_bounds.w=370.0;
    text_gameover_tile.out_bounds.h=75.0;
    text_gameover_tile.out_bounds.off_x=0.0;
    text_gameover_tile.out_bounds.off_y=0.0;
    generate_vertex_data_from_bb(text_gameover_tile.vertex_data, &text_gameover_tile.out_bounds);
    generate_texcoord_data_from_rect(text_gameover_tile.tex_data, 250.0, 900.0, text_gameover_tile.out_bounds.w, text_gameover_tile.out_bounds.h, tex_container_w, tex_container_h);

}




int text_credits_gettexture(int tex_container_w, int tex_container_h) {
    // genera una textura con los creditos para dibujar    
    
    
    void *imagebuf;
    
    int buf_w,buf_h;
    buf_w=tex_container_w;
    buf_h=tex_container_h;
    
    
//    printf("w: %u h: %u\n",buf_w,buf_h);
    imagebuf=calloc(buf_w*buf_h,4);
//    printf("imagebuf: %lx \n",(unsigned long)imagebuf);
    CGColorSpaceRef rgbcolorspace=CGColorSpaceCreateDeviceRGB();
    CGContextRef bufCtx=CGBitmapContextCreate(imagebuf, buf_w, buf_h, 8, 4*buf_w, rgbcolorspace, kCGImageAlphaPremultipliedLast);
    CGColorSpaceRelease(rgbcolorspace);
    
    // para poder llenar el fondo de la textura si quiero quitar la transparencia
    CGContextSetRGBFillColor(bufCtx, 1.0, 1.0, 1.0, 0.3);
    CGContextFillRect(bufCtx,CGRectMake(0, 424 , buf_w, /*buf_h*/600));

    
//    CGContextSetRGBFillColor(bufCtx, 1.0, 1.0, 0.7, 1.0);
//    CGContextSetRGBStrokeColor(bufCtx, 1.0, 1.0, 0.0, 1.0);
//    CGContextFillRect(bufCtx,CGRectMake(10.0, 10.0 , 60.0, 30.0));
//    CGContextStrokeRect(bufCtx,CGRectMake(10.0, 10.0 , 60.0, 30.0));

    int text_y=tex_container_h;
    char *text;
    
    CGContextSetRGBFillColor(bufCtx, 1.0, 0.0, 0.0, 1.0);
    CGContextSelectFont(bufCtx, "MarkerFelt-Wide", 70,kCGEncodingMacRoman);
    
    
    text_y-=80;
    text="Pilgrim";
    CGContextShowTextAtPoint(bufCtx, 10, text_y, text , strlen(text));

    CGContextSetRGBFillColor(bufCtx, 0.4, 0.1, 0.1, 1.0);
    CGContextSetFontSize(bufCtx, 40);
    
    char version_text[40];
    sprintf(version_text,"v%s (r%s)",pilgrim_version_number,pilgrim_build_number);
    text=version_text;
    CGContextShowTextAtPoint(bufCtx, 300, text_y, text , strlen(text));

#if PILGRIM_INCLUDE_WEBLINK
    CGContextSetFontSize(bufCtx, 24);
    CGContextSetRGBFillColor(bufCtx, 1.0, 0.0, 0.0, 1.0);
    text=PILGRIM_WEBLINK;
    CGContextShowTextAtPoint(bufCtx, 550, text_y-135, text , strlen(text));
#endif
    CGContextSetRGBFillColor(bufCtx, 0.4, 0.1, 0.1, 1.0);
//    text="[open in safari]";
//    CGContextShowTextAtPoint(bufCtx, 600, text_y-175, text , strlen(text));
    CGContextSetFontSize(bufCtx, 40);

    
    
    CGContextSetRGBFillColor(bufCtx, 0.3, 0.3, 0.1, 1.0);
    
    char *ctext[]={
        "created by Bware studios",
        "",
        "Bware studios are:",
        "  Mikel (programming and IA)",
        "  I\x96igo (graphics and concept design)",
        "  David (music and sound FX)",
        "",
        "Many thanks to all beta testers:",
        "  Silvia, Maria, Patri, Raul...",
        "",
        "Special thanks to <add your name here>",
    };
    
    int i;
    
    for (i=0;i<11; i++) {
        text_y-=50;
        text=ctext[i];
        if (strlen(text)>0) {
            CGContextShowTextAtPoint(bufCtx, 30, text_y, text , strlen(text));
        } else {
            text_y+=25;
        }
    }
    
    
    // dibuja letras para el menu en la zona no usada
    CGContextSetRGBFillColor(bufCtx, 0.1, 0.1, 0.1, 1.0);
    text="Start";
    CGContextShowTextAtPoint(bufCtx, 0, tex_container_h-700-42, text , strlen(text));
    text="Help";
    CGContextShowTextAtPoint(bufCtx, 0, tex_container_h-750-42, text , strlen(text));
    text="Credits";
    CGContextShowTextAtPoint(bufCtx, 0, tex_container_h-800-42, text , strlen(text));
    text="Hall of fame";
    CGContextShowTextAtPoint(bufCtx, 0, tex_container_h-850-42, text , strlen(text));

    CGContextSetRGBFillColor(bufCtx, 0.8, 0.1, 0.1, 1.0);
    text="Start";
    CGContextShowTextAtPoint(bufCtx, 250, tex_container_h-700-42, text , strlen(text));
    text="Help";
    CGContextShowTextAtPoint(bufCtx, 250, tex_container_h-750-42, text , strlen(text));
    text="Credits";
    CGContextShowTextAtPoint(bufCtx, 250, tex_container_h-800-42, text , strlen(text));
    text="Hall of fame";
    CGContextShowTextAtPoint(bufCtx, 250, tex_container_h-850-42, text , strlen(text));

    CGContextSetRGBFillColor(bufCtx, 1.0, 1.0, 1.0, 1.0);
    CGContextSelectFont(bufCtx, "MarkerFelt-Wide", 80,kCGEncodingMacRoman);
    text="Game Over";
    CGContextShowTextAtPoint(bufCtx, 250+4, tex_container_h-900-70, text , strlen(text));
    CGContextSetRGBFillColor(bufCtx, 1.0, 0.0, 0.0, 1.0);
    CGContextSelectFont(bufCtx, "MarkerFelt-Wide", 80,kCGEncodingMacRoman);
    text="Game Over";
    CGContextShowTextAtPoint(bufCtx, 250+4-4, tex_container_h-900-70+4, text , strlen(text));

    
    CGContextRelease(bufCtx);
    
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
        printf("text_credits_gettexture(): Error al cargar la textura e=%u\n",err);
        free(imagebuf);
        return -1;
    }
    
    free(imagebuf);
//    printf("credits tex_id = %d\n",tex_name);
    return tex_name;
}


void text_highscores_gentexture(int tex_container_w, int tex_container_h) {
    GLuint tex_name;
    if (text_highscores_tile.tex_id==-1) { 
        // si es la primera vez genera la textura opengl
        glGenTextures(1, &tex_name);
        text_highscores_tile.tex_id=tex_name;
        
        text_highscores_tile.out_bounds.w=900.0;
        text_highscores_tile.out_bounds.h=600.0;
        text_highscores_tile.out_bounds.off_x=0.0;
        text_highscores_tile.out_bounds.off_y=0.0;
        generate_vertex_data_from_bb(text_highscores_tile.vertex_data, &text_highscores_tile.out_bounds);
        generate_texcoord_data_from_rect(text_highscores_tile.tex_data, 0.0, 0.0, text_highscores_tile.out_bounds.w, text_highscores_tile.out_bounds.h, tex_container_w, tex_container_h);
    } else {
        tex_name=text_highscores_tile.tex_id;
    }

    void *imagebuf;
    
    int buf_w,buf_h;
    buf_w=tex_container_w;
    buf_h=tex_container_h;

//    printf("w: %u h: %u\n",buf_w,buf_h);
    imagebuf=calloc(buf_w*buf_h,4);
//    printf("imagebuf: %lx \n",(unsigned long)imagebuf);
    CGColorSpaceRef rgbcolorspace=CGColorSpaceCreateDeviceRGB();
    CGContextRef bufCtx=CGBitmapContextCreate(imagebuf, buf_w, buf_h, 8, 4*buf_w, rgbcolorspace, kCGImageAlphaPremultipliedLast);
    CGColorSpaceRelease(rgbcolorspace);
    
    // fondo
    CGContextSetRGBFillColor(bufCtx, .3, .3, .3, 0.9);
    CGContextFillRect(bufCtx,CGRectMake(0, 0 , buf_w, buf_h));

    // preparacion de coretext
    CTFontDescriptorRef fontdesc=CTFontDescriptorCreateWithNameAndSize(CFSTR("MarkerFelt-Wide"), 70);
    CTFontRef font70=CTFontCreateWithFontDescriptor(fontdesc, 70, NULL);
//    CFRelease(fontdesc);
//    CTFontDescriptorRef fontdesc=CTFontDescriptorCreateWithNameAndSize(CFSTR("MarkerFelt-Wide"), 70);
    CTFontRef font40=CTFontCreateWithFontDescriptor(fontdesc, 40, NULL);
    CFRelease(fontdesc);
    CFStringRef str_atrkeys[] = { kCTFontAttributeName, kCTForegroundColorFromContextAttributeName };
    CFTypeRef str_atrvalues70[] = { font70 , kCFBooleanTrue };    
    CFTypeRef str_atrvalues40[] = { font40 , kCFBooleanTrue };    
    CFDictionaryRef str_atr70=CFDictionaryCreate(NULL, (const void **)&str_atrkeys, (const void **)&str_atrvalues70, 2, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
    CFDictionaryRef str_atr40=CFDictionaryCreate(NULL, (const void **)&str_atrkeys, (const void **)&str_atrvalues40, 2, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
    
    CFAttributedStringRef astr;
    CTLineRef str_linea;
    CFStringRef cfstr;

    
    
// aqui escribir los records    

    int text_y=tex_container_h;
    char *text;
    
    CGContextSetRGBFillColor(bufCtx, 0.8, 0.0, 0.3, 1.0);
//    CGContextSelectFont(bufCtx, "MarkerFelt-Wide", 70,kCGEncodingMacRoman);
    
    text_y-=80;
    text="Hall of Fame";
    cfstr=CFStringCreateWithCString(kCFAllocatorDefault, text, kCFStringEncodingUTF8);
    astr=CFAttributedStringCreate(kCFAllocatorDefault, cfstr, str_atr70);
    str_linea=CTLineCreateWithAttributedString(astr);
    CGContextSetTextPosition(bufCtx, 10, text_y);
//    CGContextShowTextAtPoint(bufCtx, 10, text_y, text , strlen(text));
    CTLineDraw(str_linea, bufCtx);
    CFRelease(astr);
    CFRelease(str_linea);
    CFRelease(cfstr);
    
    
//    CGContextSetRGBFillColor(bufCtx, 0.8, 0.8, 0.8, 1.0);

//    CGContextSelectFont(bufCtx, "MarkerFelt-Wide", 40,kCGEncodingMacRoman);

    int i;
    
    char textbuf[100];
    for (i=0;i<HIGHSCORE_MAX; i++) {
        text_y-=50;
        
        if (game_highscore_pos==i) {
            CGContextSetRGBFillColor(bufCtx, 1.0, 0, 0, 1.0);            
        } else {
            CGContextSetRGBFillColor(bufCtx, 0.8, 0.8, 0.8, 1.0);
        }
        
        sprintf(textbuf,"%s",highscore_table[i].name);
        
        cfstr=CFStringCreateWithCString(kCFAllocatorDefault, textbuf, kCFStringEncodingUTF8);
        astr=CFAttributedStringCreate(kCFAllocatorDefault, cfstr, str_atr40);
        str_linea=CTLineCreateWithAttributedString(astr);
        CGContextSetTextPosition(bufCtx, 30, text_y);
        CTLineDraw(str_linea, bufCtx);
        CFRelease(astr);
        CFRelease(str_linea);
        CFRelease(cfstr);        
        //CGContextShowTextAtPoint(bufCtx, 30, text_y, textbuf , strlen(textbuf));
        
        sprintf(textbuf,"%d m",highscore_table[i].score);
        cfstr=CFStringCreateWithCString(kCFAllocatorDefault, textbuf, kCFStringEncodingUTF8);
        astr=CFAttributedStringCreate(kCFAllocatorDefault, cfstr, str_atr40);
        str_linea=CTLineCreateWithAttributedString(astr);
        CGContextSetTextPosition(bufCtx, 450, text_y);
        CTLineDraw(str_linea, bufCtx);
        CFRelease(astr);
        CFRelease(str_linea);
        CFRelease(cfstr);        
        
        sprintf(textbuf,"%d hits",highscore_table[i].hits);
        cfstr=CFStringCreateWithCString(kCFAllocatorDefault, textbuf, kCFStringEncodingUTF8);
        astr=CFAttributedStringCreate(kCFAllocatorDefault, cfstr, str_atr40);
        str_linea=CTLineCreateWithAttributedString(astr);
        CGContextSetTextPosition(bufCtx, 700, text_y);
        CTLineDraw(str_linea, bufCtx);
        CFRelease(astr);
        CFRelease(str_linea);
        CFRelease(cfstr);        

        
        //CGContextShowTextAtPoint(bufCtx, 650, text_y, textbuf , strlen(textbuf));
    }
    
    
    CGContextRelease(bufCtx);

    
    glBindTexture(GL_TEXTURE_2D, tex_name);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, buf_w, buf_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, imagebuf);

    GLuint err=glGetError();
    if (err != GL_NO_ERROR) {
        printf("text_highscores_gettexture(): Error al cargar la textura e=%u\n",err);
        free(imagebuf);
        return;
    }
    
    free(imagebuf);
//    printf("hof tex_id = %d\n",tex_name);

    CFRelease(font40);
    CFRelease(font70);
    CFRelease(str_atr40);
    CFRelease(str_atr70);
}

// usa la misma textura que los highscores porque nunca se dibujan a la vez
void text_results_gentexture(int tex_container_w, int tex_container_h) {
    GLuint tex_name;
    if (text_highscores_tile.tex_id==-1) { 
        // si es la primera vez genera la textura opengl
        glGenTextures(1, &tex_name);
        text_highscores_tile.tex_id=tex_name;
        
        text_highscores_tile.out_bounds.w=900.0;
        text_highscores_tile.out_bounds.h=600.0;
        text_highscores_tile.out_bounds.off_x=0.0;
        text_highscores_tile.out_bounds.off_y=0.0;
        generate_vertex_data_from_bb(text_highscores_tile.vertex_data, &text_highscores_tile.out_bounds);
        generate_texcoord_data_from_rect(text_highscores_tile.tex_data, 0.0, 0.0, text_highscores_tile.out_bounds.w, text_highscores_tile.out_bounds.h, tex_container_w, tex_container_h);
    } else {
        tex_name=text_highscores_tile.tex_id;
    }
    
    void *imagebuf;
    
    int buf_w,buf_h;
    buf_w=tex_container_w;
    buf_h=tex_container_h;
    
//    printf("w: %u h: %u\n",buf_w,buf_h);
    imagebuf=calloc(buf_w*buf_h,4);
//    printf("imagebuf: %lx \n",(unsigned long)imagebuf);
    CGColorSpaceRef rgbcolorspace=CGColorSpaceCreateDeviceRGB();
    CGContextRef bufCtx=CGBitmapContextCreate(imagebuf, buf_w, buf_h, 8, 4*buf_w, rgbcolorspace, kCGImageAlphaPremultipliedLast);
    CGColorSpaceRelease(rgbcolorspace);
    
//    // fondo
//    CGContextSetRGBFillColor(bufCtx, .6, .6, .6, 0.4);
//    CGContextFillRect(bufCtx,CGRectMake(0, 0 , buf_w, buf_h));
    
    // preparacion de coretext
    CTFontDescriptorRef fontdesc=CTFontDescriptorCreateWithNameAndSize(CFSTR("MarkerFelt-Wide"), 60);
    CTFontRef font70=CTFontCreateWithFontDescriptor(fontdesc, 60, NULL);
    //    CFRelease(fontdesc);
    //    CTFontDescriptorRef fontdesc=CTFontDescriptorCreateWithNameAndSize(CFSTR("MarkerFelt-Wide"), 70);
    CTFontRef font40=CTFontCreateWithFontDescriptor(fontdesc, 40, NULL);
    CFRelease(fontdesc);
    CFStringRef str_atrkeys[] = { kCTFontAttributeName, kCTForegroundColorFromContextAttributeName };
    CFTypeRef str_atrvalues70[] = { font70 , kCFBooleanTrue };    
    CFTypeRef str_atrvalues40[] = { font40 , kCFBooleanTrue };    
    CFDictionaryRef str_atr70=CFDictionaryCreate(NULL, (const void **)&str_atrkeys, (const void **)&str_atrvalues70, 2, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
    CFDictionaryRef str_atr40=CFDictionaryCreate(NULL, (const void **)&str_atrkeys, (const void **)&str_atrvalues40, 2, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
    
    CFAttributedStringRef astr;
    CTLineRef str_linea;
    CFStringRef cfstr;
    
    
    
    
    int text_y=tex_container_h;
    char textbuf[100];
    
    CGContextSetRGBFillColor(bufCtx, 0.3, 0.2, 0.2, 1.0);
    
    text_y-=10;
    float linestep=80;
    text_y-=linestep;
    text_y-=linestep;
    stats_sprintf_distancia(textbuf);
    cfstr=CFStringCreateWithCString(kCFAllocatorDefault, textbuf, kCFStringEncodingUTF8);
    astr=CFAttributedStringCreate(kCFAllocatorDefault, cfstr, str_atr70);
    str_linea=CTLineCreateWithAttributedString(astr);
    CGContextSetTextPosition(bufCtx, 0, text_y);
    //    CGContextShowTextAtPoint(bufCtx, 10, text_y, text , strlen(text));
    if (game_highscore_pos!=-1) {
        CGContextSetRGBFillColor(bufCtx, 1.0, 0.0, 0.2, 1.0);
    }
    CTLineDraw(str_linea, bufCtx);
    if (game_highscore_pos!=-1) {
        CGContextSetRGBFillColor(bufCtx, 0.3, 0.2, 0.2, 1.0);
    }
    CFRelease(astr);
    CFRelease(str_linea);
    CFRelease(cfstr);
    
//    text_y-=linestep;
    stats_sprintf_time(textbuf);
    cfstr=CFStringCreateWithCString(kCFAllocatorDefault, textbuf, kCFStringEncodingUTF8);
    astr=CFAttributedStringCreate(kCFAllocatorDefault, cfstr, str_atr70);
    str_linea=CTLineCreateWithAttributedString(astr);
    CGContextSetTextPosition(bufCtx, 250, text_y);
    //    CGContextShowTextAtPoint(bufCtx, 10, text_y, text , strlen(text));
    CTLineDraw(str_linea, bufCtx);
    CFRelease(astr);
    CFRelease(str_linea);
    CFRelease(cfstr);
    
//    text_y-=linestep;
//    stats_sprintf_speed(textbuf);
//    cfstr=CFStringCreateWithCString(kCFAllocatorDefault, textbuf, kCFStringEncodingUTF8);
//    astr=CFAttributedStringCreate(kCFAllocatorDefault, cfstr, str_atr70);
//    str_linea=CTLineCreateWithAttributedString(astr);
//    CGContextSetTextPosition(bufCtx, 610, text_y);
//    //    CGContextShowTextAtPoint(bufCtx, 10, text_y, text , strlen(text));
//    CTLineDraw(str_linea, bufCtx);
//    CFRelease(astr);
//    CFRelease(str_linea);
//    CFRelease(cfstr);

//    text_y-=2*linestep;
//    sprintf(textbuf,"Hits:");
//    cfstr=CFStringCreateWithCString(kCFAllocatorDefault, textbuf, kCFStringEncodingUTF8);
//    astr=CFAttributedStringCreate(kCFAllocatorDefault, cfstr, str_atr70);
//    str_linea=CTLineCreateWithAttributedString(astr);
//    CGContextSetTextPosition(bufCtx, 100, text_y);
//    //    CGContextShowTextAtPoint(bufCtx, 10, text_y, text , strlen(text));
//    CTLineDraw(str_linea, bufCtx);
//    CFRelease(astr);
//    CFRelease(str_linea);
//    CFRelease(cfstr);
    stats_sprintf_hits(textbuf,1);
    cfstr=CFStringCreateWithCString(kCFAllocatorDefault, textbuf, kCFStringEncodingUTF8);
    astr=CFAttributedStringCreate(kCFAllocatorDefault, cfstr, str_atr70);
    str_linea=CTLineCreateWithAttributedString(astr);
    CGContextSetTextPosition(bufCtx, 490, text_y);
    //    CGContextShowTextAtPoint(bufCtx, 10, text_y, text , strlen(text));
    CTLineDraw(str_linea, bufCtx);
    CFRelease(astr);
    CFRelease(str_linea);
    CFRelease(cfstr);
    stats_sprintf_hits(textbuf,2);
    cfstr=CFStringCreateWithCString(kCFAllocatorDefault, textbuf, kCFStringEncodingUTF8);
    astr=CFAttributedStringCreate(kCFAllocatorDefault, cfstr, str_atr70);
    str_linea=CTLineCreateWithAttributedString(astr);
    CGContextSetTextPosition(bufCtx, 610, text_y);
    //    CGContextShowTextAtPoint(bufCtx, 10, text_y, text , strlen(text));
    CTLineDraw(str_linea, bufCtx);
    CFRelease(astr);
    CFRelease(str_linea);
    CFRelease(cfstr);

//    //text_y-=linestep;
//    stats_sprintf_caidas(textbuf);
//    cfstr=CFStringCreateWithCString(kCFAllocatorDefault, textbuf, kCFStringEncodingUTF8);
//    astr=CFAttributedStringCreate(kCFAllocatorDefault, cfstr, str_atr70);
//    str_linea=CTLineCreateWithAttributedString(astr);
//    CGContextSetTextPosition(bufCtx, 700, text_y);
//    //    CGContextShowTextAtPoint(bufCtx, 10, text_y, text , strlen(text));
//    CTLineDraw(str_linea, bufCtx);
//    CFRelease(astr);
//    CFRelease(str_linea);
//    CFRelease(cfstr);

    stats_sprintf_comida(textbuf,0);
    cfstr=CFStringCreateWithCString(kCFAllocatorDefault, textbuf, kCFStringEncodingUTF8);
    astr=CFAttributedStringCreate(kCFAllocatorDefault, cfstr, str_atr70);
    str_linea=CTLineCreateWithAttributedString(astr);
    CGContextSetTextPosition(bufCtx, 780, text_y);
    //    CGContextShowTextAtPoint(bufCtx, 10, text_y, text , strlen(text));
    CTLineDraw(str_linea, bufCtx);
    CFRelease(astr);
    CFRelease(str_linea);
    CFRelease(cfstr);

    
    
    
    CGContextRelease(bufCtx);
    
    
    glBindTexture(GL_TEXTURE_2D, tex_name);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, buf_w, buf_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, imagebuf);
    
    GLuint err=glGetError();
    if (err != GL_NO_ERROR) {
        printf("text_highscores_gettexture(): Error al cargar la textura e=%u\n",err);
        free(imagebuf);
        return;
    }
    
    free(imagebuf);
//    printf("hof tex_id = %d\n",tex_name);
    
    CFRelease(font40);
    CFRelease(font70);
    CFRelease(str_atr40);
    CFRelease(str_atr70);
}


