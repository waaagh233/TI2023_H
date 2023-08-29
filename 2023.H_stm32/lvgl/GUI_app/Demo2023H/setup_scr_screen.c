/*
 * Copyright 2023 NXP
 * SPDX-License-Identifier: MIT
 */

#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"
#include "events_init.h"
//#include "custom.h"


void setup_scr_screen(lv_ui *ui){

	//Write codes screen
	ui->screen = lv_obj_create(NULL);

	//Write style state: LV_STATE_DEFAULT for style_screen_main_main_default
	static lv_style_t style_screen_main_main_default;
	lv_style_reset(&style_screen_main_main_default);
	lv_style_set_bg_color(&style_screen_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_opa(&style_screen_main_main_default, 255);
	lv_obj_add_style(ui->screen, &style_screen_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_1
	ui->screen_label_1 = lv_label_create(ui->screen);
	lv_obj_set_pos(ui->screen_label_1, 350, 100);
	lv_obj_set_size(ui->screen_label_1, 200, 32);
	lv_label_set_text(ui->screen_label_1, "输入信号A频率：");
	lv_label_set_long_mode(ui->screen_label_1, LV_LABEL_LONG_WRAP);
	lv_obj_set_style_text_align(ui->screen_label_1, LV_TEXT_ALIGN_RIGHT, 0);

	//Write style state: LV_STATE_DEFAULT for style_screen_label_1_main_main_default
	static lv_style_t style_screen_label_1_main_main_default;
	lv_style_reset(&style_screen_label_1_main_main_default);
	lv_style_set_radius(&style_screen_label_1_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_label_1_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_label_1_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_label_1_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_label_1_main_main_default, 0);
	lv_style_set_text_color(&style_screen_label_1_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_label_1_main_main_default, &lv_font_simsun_24);
	lv_style_set_text_letter_space(&style_screen_label_1_main_main_default, 2);
	lv_style_set_pad_left(&style_screen_label_1_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_label_1_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_label_1_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_label_1_main_main_default, 0);
	lv_obj_add_style(ui->screen_label_1, &style_screen_label_1_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_2
	ui->screen_label_2 = lv_label_create(ui->screen);
	lv_obj_set_pos(ui->screen_label_2, 350, 140);
	lv_obj_set_size(ui->screen_label_2, 200, 32);
	lv_label_set_text(ui->screen_label_2, "输入信号B频率：");
	lv_label_set_long_mode(ui->screen_label_2, LV_LABEL_LONG_WRAP);
	lv_obj_set_style_text_align(ui->screen_label_2, LV_TEXT_ALIGN_RIGHT, 0);

	//Write style state: LV_STATE_DEFAULT for style_screen_label_2_main_main_default
	static lv_style_t style_screen_label_2_main_main_default;
	lv_style_reset(&style_screen_label_2_main_main_default);
	lv_style_set_radius(&style_screen_label_2_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_label_2_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_label_2_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_label_2_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_label_2_main_main_default, 0);
	lv_style_set_text_color(&style_screen_label_2_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_label_2_main_main_default, &lv_font_simsun_24);
	lv_style_set_text_letter_space(&style_screen_label_2_main_main_default, 2);
	lv_style_set_pad_left(&style_screen_label_2_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_label_2_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_label_2_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_label_2_main_main_default, 0);
	lv_obj_add_style(ui->screen_label_2, &style_screen_label_2_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_3
	ui->screen_label_3 = lv_label_create(ui->screen);
	lv_obj_set_pos(ui->screen_label_3, 350, 180);
	lv_obj_set_size(ui->screen_label_3, 200, 32);
	lv_label_set_text(ui->screen_label_3, "输入信号A波形：");
	lv_label_set_long_mode(ui->screen_label_3, LV_LABEL_LONG_WRAP);
	lv_obj_set_style_text_align(ui->screen_label_3, LV_TEXT_ALIGN_RIGHT, 0);

	//Write style state: LV_STATE_DEFAULT for style_screen_label_3_main_main_default
	static lv_style_t style_screen_label_3_main_main_default;
	lv_style_reset(&style_screen_label_3_main_main_default);
	lv_style_set_radius(&style_screen_label_3_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_label_3_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_label_3_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_label_3_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_label_3_main_main_default, 0);
	lv_style_set_text_color(&style_screen_label_3_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_label_3_main_main_default, &lv_font_simsun_24);
	lv_style_set_text_letter_space(&style_screen_label_3_main_main_default, 2);
	lv_style_set_pad_left(&style_screen_label_3_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_label_3_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_label_3_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_label_3_main_main_default, 0);
	lv_obj_add_style(ui->screen_label_3, &style_screen_label_3_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_4
	ui->screen_label_4 = lv_label_create(ui->screen);
	lv_obj_set_pos(ui->screen_label_4, 350, 220);
	lv_obj_set_size(ui->screen_label_4, 200, 32);
	lv_label_set_text(ui->screen_label_4, "输入信号B波形：");
	lv_label_set_long_mode(ui->screen_label_4, LV_LABEL_LONG_WRAP);
	lv_obj_set_style_text_align(ui->screen_label_4, LV_TEXT_ALIGN_RIGHT, 0);

	//Write style state: LV_STATE_DEFAULT for style_screen_label_4_main_main_default
	static lv_style_t style_screen_label_4_main_main_default;
	lv_style_reset(&style_screen_label_4_main_main_default);
	lv_style_set_radius(&style_screen_label_4_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_label_4_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_label_4_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_label_4_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_label_4_main_main_default, 0);
	lv_style_set_text_color(&style_screen_label_4_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_label_4_main_main_default, &lv_font_simsun_24);
	lv_style_set_text_letter_space(&style_screen_label_4_main_main_default, 2);
	lv_style_set_pad_left(&style_screen_label_4_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_label_4_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_label_4_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_label_4_main_main_default, 0);
	lv_obj_add_style(ui->screen_label_4, &style_screen_label_4_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_5
	ui->screen_label_5 = lv_label_create(ui->screen);
	lv_obj_set_pos(ui->screen_label_5, 350, 260);
	lv_obj_set_size(ui->screen_label_5, 200, 32);
	lv_label_set_text(ui->screen_label_5, "设置初相差：");
	lv_label_set_long_mode(ui->screen_label_5, LV_LABEL_LONG_WRAP);
	lv_obj_set_style_text_align(ui->screen_label_5, LV_TEXT_ALIGN_RIGHT, 0);

	//Write style state: LV_STATE_DEFAULT for style_screen_label_5_main_main_default
	static lv_style_t style_screen_label_5_main_main_default;
	lv_style_reset(&style_screen_label_5_main_main_default);
	lv_style_set_radius(&style_screen_label_5_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_label_5_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_label_5_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_label_5_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_label_5_main_main_default, 0);
	lv_style_set_text_color(&style_screen_label_5_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_label_5_main_main_default, &lv_font_simsun_24);
	lv_style_set_text_letter_space(&style_screen_label_5_main_main_default, 2);
	lv_style_set_pad_left(&style_screen_label_5_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_label_5_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_label_5_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_label_5_main_main_default, 0);
	lv_obj_add_style(ui->screen_label_5, &style_screen_label_5_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_6
	ui->screen_label_6 = lv_label_create(ui->screen);
	lv_obj_set_pos(ui->screen_label_6, 300, 350);
	lv_obj_set_size(ui->screen_label_6, 120, 32);
	lv_label_set_text(ui->screen_label_6, "正常模式");
	lv_label_set_long_mode(ui->screen_label_6, LV_LABEL_LONG_WRAP);
	lv_obj_set_style_text_align(ui->screen_label_6, LV_TEXT_ALIGN_CENTER, 0);

	//Write style state: LV_STATE_DEFAULT for style_screen_label_6_main_main_default
	static lv_style_t style_screen_label_6_main_main_default;
	lv_style_reset(&style_screen_label_6_main_main_default);
	lv_style_set_radius(&style_screen_label_6_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_label_6_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_label_6_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_label_6_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_label_6_main_main_default, 0);
	lv_style_set_text_color(&style_screen_label_6_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_label_6_main_main_default, &lv_font_simsun_24);
	lv_style_set_text_letter_space(&style_screen_label_6_main_main_default, 2);
	lv_style_set_pad_left(&style_screen_label_6_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_label_6_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_label_6_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_label_6_main_main_default, 0);
	lv_obj_add_style(ui->screen_label_6, &style_screen_label_6_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_7
	ui->screen_label_7 = lv_label_create(ui->screen);
	lv_obj_set_pos(ui->screen_label_7, 445, 350);
	lv_obj_set_size(ui->screen_label_7, 120, 32);
	lv_label_set_text(ui->screen_label_7, "高频模式");
	lv_label_set_long_mode(ui->screen_label_7, LV_LABEL_LONG_WRAP);
	lv_obj_set_style_text_align(ui->screen_label_7, LV_TEXT_ALIGN_CENTER, 0);

	//Write style state: LV_STATE_DEFAULT for style_screen_label_7_main_main_default
	static lv_style_t style_screen_label_7_main_main_default;
	lv_style_reset(&style_screen_label_7_main_main_default);
	lv_style_set_radius(&style_screen_label_7_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_label_7_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_label_7_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_label_7_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_label_7_main_main_default, 0);
	lv_style_set_text_color(&style_screen_label_7_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_label_7_main_main_default, &lv_font_simsun_24);
	lv_style_set_text_letter_space(&style_screen_label_7_main_main_default, 2);
	lv_style_set_pad_left(&style_screen_label_7_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_label_7_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_label_7_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_label_7_main_main_default, 0);
	lv_obj_add_style(ui->screen_label_7, &style_screen_label_7_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_8
	ui->screen_label_8 = lv_label_create(ui->screen);
	lv_obj_set_pos(ui->screen_label_8, 580, 350);
	lv_obj_set_size(ui->screen_label_8, 150, 32);
	lv_label_set_text(ui->screen_label_8, "高精度模式");
	lv_label_set_long_mode(ui->screen_label_8, LV_LABEL_LONG_WRAP);
	lv_obj_set_style_text_align(ui->screen_label_8, LV_TEXT_ALIGN_CENTER, 0);

	//Write style state: LV_STATE_DEFAULT for style_screen_label_8_main_main_default
	static lv_style_t style_screen_label_8_main_main_default;
	lv_style_reset(&style_screen_label_8_main_main_default);
	lv_style_set_radius(&style_screen_label_8_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_label_8_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_label_8_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_label_8_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_label_8_main_main_default, 0);
	lv_style_set_text_color(&style_screen_label_8_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_label_8_main_main_default, &lv_font_simsun_24);
	lv_style_set_text_letter_space(&style_screen_label_8_main_main_default, 2);
	lv_style_set_pad_left(&style_screen_label_8_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_label_8_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_label_8_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_label_8_main_main_default, 0);
	lv_obj_add_style(ui->screen_label_8, &style_screen_label_8_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_btn_1
	ui->screen_btn_1 = lv_btn_create(ui->screen);
	lv_obj_set_pos(ui->screen_btn_1, 310, 390);
	lv_obj_set_size(ui->screen_btn_1, 100, 50);

	//Write style state: LV_STATE_DEFAULT for style_screen_btn_1_main_main_default
	static lv_style_t style_screen_btn_1_main_main_default;
	lv_style_reset(&style_screen_btn_1_main_main_default);
	lv_style_set_radius(&style_screen_btn_1_main_main_default, 5);
	lv_style_set_bg_color(&style_screen_btn_1_main_main_default, lv_color_make(0xff, 0xff, 0x00));
	lv_style_set_bg_grad_color(&style_screen_btn_1_main_main_default, lv_color_make(0xff, 0xff, 0x00));
	lv_style_set_bg_grad_dir(&style_screen_btn_1_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_btn_1_main_main_default, 255);
	lv_style_set_shadow_color(&style_screen_btn_1_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_shadow_opa(&style_screen_btn_1_main_main_default, 0);
	lv_style_set_border_color(&style_screen_btn_1_main_main_default, lv_color_make(0xff, 0xff, 0x00));
	lv_style_set_border_width(&style_screen_btn_1_main_main_default, 0);
	lv_style_set_border_opa(&style_screen_btn_1_main_main_default, 255);
	lv_obj_add_style(ui->screen_btn_1, &style_screen_btn_1_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	ui->screen_btn_1_label = lv_label_create(ui->screen_btn_1);
	lv_label_set_text(ui->screen_btn_1_label, "START");
	lv_obj_set_style_text_color(ui->screen_btn_1_label, lv_color_make(0x00, 0x00, 0x00), LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_btn_1_label, &lv_font_simsun_24, LV_STATE_DEFAULT);
	lv_obj_set_style_pad_all(ui->screen_btn_1, 0, LV_STATE_DEFAULT);
	lv_obj_align(ui->screen_btn_1_label, LV_ALIGN_CENTER, 0, 0);

	//Write codes screen_btn_2
	ui->screen_btn_2 = lv_btn_create(ui->screen);
	lv_obj_set_pos(ui->screen_btn_2, 455, 390);
	lv_obj_set_size(ui->screen_btn_2, 100, 50);

	//Write style state: LV_STATE_DEFAULT for style_screen_btn_2_main_main_default
	static lv_style_t style_screen_btn_2_main_main_default;
	lv_style_reset(&style_screen_btn_2_main_main_default);
	lv_style_set_radius(&style_screen_btn_2_main_main_default, 5);
	lv_style_set_bg_color(&style_screen_btn_2_main_main_default, lv_color_make(0xff, 0xff, 0x00));
	lv_style_set_bg_grad_color(&style_screen_btn_2_main_main_default, lv_color_make(0xff, 0xff, 0x00));
	lv_style_set_bg_grad_dir(&style_screen_btn_2_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_btn_2_main_main_default, 255);
	lv_style_set_shadow_color(&style_screen_btn_2_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_shadow_opa(&style_screen_btn_2_main_main_default, 0);
	lv_style_set_border_color(&style_screen_btn_2_main_main_default, lv_color_make(0xff, 0xff, 0x00));
	lv_style_set_border_width(&style_screen_btn_2_main_main_default, 0);
	lv_style_set_border_opa(&style_screen_btn_2_main_main_default, 255);
	lv_obj_add_style(ui->screen_btn_2, &style_screen_btn_2_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	ui->screen_btn_2_label = lv_label_create(ui->screen_btn_2);
	lv_label_set_text(ui->screen_btn_2_label, "ON");
	lv_obj_set_style_text_color(ui->screen_btn_2_label, lv_color_make(0x00, 0x00, 0x00), LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_btn_2_label, &lv_font_simsun_24, LV_STATE_DEFAULT);
	lv_obj_set_style_pad_all(ui->screen_btn_2, 0, LV_STATE_DEFAULT);
	lv_obj_align(ui->screen_btn_2_label, LV_ALIGN_CENTER, 0, 0);

	//Write codes screen_btn_3
	ui->screen_btn_3 = lv_btn_create(ui->screen);
	lv_obj_set_pos(ui->screen_btn_3, 600, 390);
	lv_obj_set_size(ui->screen_btn_3, 100, 50);

	//Write style state: LV_STATE_DEFAULT for style_screen_btn_3_main_main_default
	static lv_style_t style_screen_btn_3_main_main_default;
	lv_style_reset(&style_screen_btn_3_main_main_default);
	lv_style_set_radius(&style_screen_btn_3_main_main_default, 5);
	lv_style_set_bg_color(&style_screen_btn_3_main_main_default, lv_color_make(0xff, 0xff, 0x00));
	lv_style_set_bg_grad_color(&style_screen_btn_3_main_main_default, lv_color_make(0xff, 0xff, 0x00));
	lv_style_set_bg_grad_dir(&style_screen_btn_3_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_btn_3_main_main_default, 255);
	lv_style_set_shadow_color(&style_screen_btn_3_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_shadow_opa(&style_screen_btn_3_main_main_default, 0);
	lv_style_set_border_color(&style_screen_btn_3_main_main_default, lv_color_make(0xff, 0xff, 0x00));
	lv_style_set_border_width(&style_screen_btn_3_main_main_default, 0);
	lv_style_set_border_opa(&style_screen_btn_3_main_main_default, 255);
	lv_obj_add_style(ui->screen_btn_3, &style_screen_btn_3_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	ui->screen_btn_3_label = lv_label_create(ui->screen_btn_3);
	lv_label_set_text(ui->screen_btn_3_label, "ON");
	lv_obj_set_style_text_color(ui->screen_btn_3_label, lv_color_make(0x00, 0x00, 0x00), LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_btn_3_label, &lv_font_simsun_24, LV_STATE_DEFAULT);
	lv_obj_set_style_pad_all(ui->screen_btn_3, 0, LV_STATE_DEFAULT);
	lv_obj_align(ui->screen_btn_3_label, LV_ALIGN_CENTER, 0, 0);

	//Write codes screen_label_9
	ui->screen_label_9 = lv_label_create(ui->screen);
	lv_obj_set_pos(ui->screen_label_9, 340, 465);
	lv_obj_set_size(ui->screen_label_9, 200, 32);
	lv_label_set_text(ui->screen_label_9, "设置幅值");
	lv_label_set_long_mode(ui->screen_label_9, LV_LABEL_LONG_WRAP);
	lv_obj_set_style_text_align(ui->screen_label_9, LV_TEXT_ALIGN_CENTER, 0);

	//Write style state: LV_STATE_DEFAULT for style_screen_label_9_main_main_default
	static lv_style_t style_screen_label_9_main_main_default;
	lv_style_reset(&style_screen_label_9_main_main_default);
	lv_style_set_radius(&style_screen_label_9_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_label_9_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_label_9_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_label_9_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_label_9_main_main_default, 0);
	lv_style_set_text_color(&style_screen_label_9_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_label_9_main_main_default, &lv_font_simsun_24);
	lv_style_set_text_letter_space(&style_screen_label_9_main_main_default, 2);
	lv_style_set_pad_left(&style_screen_label_9_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_label_9_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_label_9_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_label_9_main_main_default, 0);
	lv_obj_add_style(ui->screen_label_9, &style_screen_label_9_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_slider_1
	ui->screen_slider_1 = lv_slider_create(ui->screen);
	lv_obj_set_pos(ui->screen_slider_1, 330, 311);
	lv_obj_set_size(ui->screen_slider_1, 359, 9);

	//Write style state: LV_STATE_DEFAULT for style_screen_slider_1_main_main_default
	static lv_style_t style_screen_slider_1_main_main_default;
	lv_style_reset(&style_screen_slider_1_main_main_default);
	lv_style_set_radius(&style_screen_slider_1_main_main_default, 50);
	lv_style_set_bg_color(&style_screen_slider_1_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_slider_1_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_slider_1_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_slider_1_main_main_default, 100);
	lv_style_set_outline_color(&style_screen_slider_1_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_outline_width(&style_screen_slider_1_main_main_default, 0);
	lv_style_set_outline_opa(&style_screen_slider_1_main_main_default, 255);
	lv_style_set_pad_left(&style_screen_slider_1_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_slider_1_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_slider_1_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_slider_1_main_main_default, 0);
	lv_obj_add_style(ui->screen_slider_1, &style_screen_slider_1_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_DEFAULT for style_screen_slider_1_main_indicator_default
	static lv_style_t style_screen_slider_1_main_indicator_default;
	lv_style_reset(&style_screen_slider_1_main_indicator_default);
	lv_style_set_radius(&style_screen_slider_1_main_indicator_default, 50);
	lv_style_set_bg_color(&style_screen_slider_1_main_indicator_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_slider_1_main_indicator_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_slider_1_main_indicator_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_slider_1_main_indicator_default, 255);
	lv_obj_add_style(ui->screen_slider_1, &style_screen_slider_1_main_indicator_default, LV_PART_INDICATOR|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_DEFAULT for style_screen_slider_1_main_knob_default
	static lv_style_t style_screen_slider_1_main_knob_default;
	lv_style_reset(&style_screen_slider_1_main_knob_default);
	lv_style_set_radius(&style_screen_slider_1_main_knob_default, 50);
	lv_style_set_bg_color(&style_screen_slider_1_main_knob_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_slider_1_main_knob_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_slider_1_main_knob_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_slider_1_main_knob_default, 255);
	lv_obj_add_style(ui->screen_slider_1, &style_screen_slider_1_main_knob_default, LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_slider_set_range(ui->screen_slider_1,0, 360);
	lv_slider_set_value(ui->screen_slider_1,0,false);

	//Write codes screen_label_10
	ui->screen_label_10 = lv_label_create(ui->screen);
	lv_obj_set_pos(ui->screen_label_10, 540, 100);
	lv_obj_set_size(ui->screen_label_10, 150, 32);
	lv_label_set_text(ui->screen_label_10, "");
	lv_label_set_long_mode(ui->screen_label_10, LV_LABEL_LONG_WRAP);
	lv_obj_set_style_text_align(ui->screen_label_10, LV_TEXT_ALIGN_LEFT, 0);

	//Write style state: LV_STATE_DEFAULT for style_screen_label_10_main_main_default
	static lv_style_t style_screen_label_10_main_main_default;
	lv_style_reset(&style_screen_label_10_main_main_default);
	lv_style_set_radius(&style_screen_label_10_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_label_10_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_label_10_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_label_10_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_label_10_main_main_default, 0);
	lv_style_set_text_color(&style_screen_label_10_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_label_10_main_main_default, &lv_font_simsun_24);
	lv_style_set_text_letter_space(&style_screen_label_10_main_main_default, 2);
	lv_style_set_pad_left(&style_screen_label_10_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_label_10_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_label_10_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_label_10_main_main_default, 0);
	lv_obj_add_style(ui->screen_label_10, &style_screen_label_10_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_11
	ui->screen_label_11 = lv_label_create(ui->screen);
	lv_obj_set_pos(ui->screen_label_11, 540, 140);
	lv_obj_set_size(ui->screen_label_11, 150, 32);
	lv_label_set_text(ui->screen_label_11, "");
	lv_label_set_long_mode(ui->screen_label_11, LV_LABEL_LONG_WRAP);
	lv_obj_set_style_text_align(ui->screen_label_11, LV_TEXT_ALIGN_LEFT, 0);

	//Write style state: LV_STATE_DEFAULT for style_screen_label_11_main_main_default
	static lv_style_t style_screen_label_11_main_main_default;
	lv_style_reset(&style_screen_label_11_main_main_default);
	lv_style_set_radius(&style_screen_label_11_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_label_11_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_label_11_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_label_11_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_label_11_main_main_default, 0);
	lv_style_set_text_color(&style_screen_label_11_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_label_11_main_main_default, &lv_font_simsun_24);
	lv_style_set_text_letter_space(&style_screen_label_11_main_main_default, 2);
	lv_style_set_pad_left(&style_screen_label_11_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_label_11_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_label_11_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_label_11_main_main_default, 0);
	lv_obj_add_style(ui->screen_label_11, &style_screen_label_11_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_12
	ui->screen_label_12 = lv_label_create(ui->screen);
	lv_obj_set_pos(ui->screen_label_12, 540, 180);
	lv_obj_set_size(ui->screen_label_12, 150, 32);
	lv_label_set_text(ui->screen_label_12, "");
	lv_label_set_long_mode(ui->screen_label_12, LV_LABEL_LONG_WRAP);
	lv_obj_set_style_text_align(ui->screen_label_12, LV_TEXT_ALIGN_LEFT, 0);

	//Write style state: LV_STATE_DEFAULT for style_screen_label_12_main_main_default
	static lv_style_t style_screen_label_12_main_main_default;
	lv_style_reset(&style_screen_label_12_main_main_default);
	lv_style_set_radius(&style_screen_label_12_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_label_12_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_label_12_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_label_12_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_label_12_main_main_default, 0);
	lv_style_set_text_color(&style_screen_label_12_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_label_12_main_main_default, &lv_font_simsun_24);
	lv_style_set_text_letter_space(&style_screen_label_12_main_main_default, 2);
	lv_style_set_pad_left(&style_screen_label_12_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_label_12_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_label_12_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_label_12_main_main_default, 0);
	lv_obj_add_style(ui->screen_label_12, &style_screen_label_12_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_13
	ui->screen_label_13 = lv_label_create(ui->screen);
	lv_obj_set_pos(ui->screen_label_13, 540, 220);
	lv_obj_set_size(ui->screen_label_13, 150, 32);
	lv_label_set_text(ui->screen_label_13, "");
	lv_label_set_long_mode(ui->screen_label_13, LV_LABEL_LONG_WRAP);
	lv_obj_set_style_text_align(ui->screen_label_13, LV_TEXT_ALIGN_LEFT, 0);

	//Write style state: LV_STATE_DEFAULT for style_screen_label_13_main_main_default
	static lv_style_t style_screen_label_13_main_main_default;
	lv_style_reset(&style_screen_label_13_main_main_default);
	lv_style_set_radius(&style_screen_label_13_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_label_13_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_label_13_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_label_13_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_label_13_main_main_default, 0);
	lv_style_set_text_color(&style_screen_label_13_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_label_13_main_main_default, &lv_font_simsun_24);
	lv_style_set_text_letter_space(&style_screen_label_13_main_main_default, 2);
	lv_style_set_pad_left(&style_screen_label_13_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_label_13_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_label_13_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_label_13_main_main_default, 0);
	lv_obj_add_style(ui->screen_label_13, &style_screen_label_13_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_btn_4
	ui->screen_btn_4 = lv_btn_create(ui->screen);
	lv_obj_set_pos(ui->screen_btn_4, 460, 510);
	lv_obj_set_size(ui->screen_btn_4, 100, 50);

	//Write style state: LV_STATE_DEFAULT for style_screen_btn_4_main_main_default
	static lv_style_t style_screen_btn_4_main_main_default;
	lv_style_reset(&style_screen_btn_4_main_main_default);
	lv_style_set_radius(&style_screen_btn_4_main_main_default, 5);
	lv_style_set_bg_color(&style_screen_btn_4_main_main_default, lv_color_make(0x00, 0xff, 0xff));
	lv_style_set_bg_grad_color(&style_screen_btn_4_main_main_default, lv_color_make(0x00, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_screen_btn_4_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_btn_4_main_main_default, 255);
	lv_style_set_shadow_color(&style_screen_btn_4_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_shadow_opa(&style_screen_btn_4_main_main_default, 0);
	lv_style_set_border_color(&style_screen_btn_4_main_main_default, lv_color_make(0x00, 0xff, 0xff));
	lv_style_set_border_width(&style_screen_btn_4_main_main_default, 0);
	lv_style_set_border_opa(&style_screen_btn_4_main_main_default, 255);
	lv_obj_add_style(ui->screen_btn_4, &style_screen_btn_4_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	ui->screen_btn_4_label = lv_label_create(ui->screen_btn_4);
	lv_label_set_text(ui->screen_btn_4_label, "调幅");
	lv_obj_set_style_text_color(ui->screen_btn_4_label, lv_color_make(0x00, 0x00, 0x00), LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_btn_4_label, &lv_font_simsun_24, LV_STATE_DEFAULT);
	lv_obj_set_style_pad_all(ui->screen_btn_4, 0, LV_STATE_DEFAULT);
	lv_obj_align(ui->screen_btn_4_label, LV_ALIGN_CENTER, 0, 0);

	//Write codes screen_label_14
	ui->screen_label_14 = lv_label_create(ui->screen);
	lv_obj_set_pos(ui->screen_label_14, 552, 260);
	lv_obj_set_size(ui->screen_label_14, 100, 32);
	lv_label_set_text(ui->screen_label_14, "0");
	lv_label_set_long_mode(ui->screen_label_14, LV_LABEL_LONG_WRAP);
	lv_obj_set_style_text_align(ui->screen_label_14, LV_TEXT_ALIGN_LEFT, 0);

	//Write style state: LV_STATE_DEFAULT for style_screen_label_14_main_main_default
	static lv_style_t style_screen_label_14_main_main_default;
	lv_style_reset(&style_screen_label_14_main_main_default);
	lv_style_set_radius(&style_screen_label_14_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_label_14_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_label_14_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_label_14_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_label_14_main_main_default, 0);
	lv_style_set_text_color(&style_screen_label_14_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_label_14_main_main_default, &lv_font_simsun_24);
	lv_style_set_text_letter_space(&style_screen_label_14_main_main_default, 2);
	lv_style_set_pad_left(&style_screen_label_14_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_label_14_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_label_14_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_label_14_main_main_default, 0);
	lv_obj_add_style(ui->screen_label_14, &style_screen_label_14_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_slider_2
	ui->screen_slider_2 = lv_slider_create(ui->screen);
	lv_obj_set_pos(ui->screen_slider_2, 500, 473);
	lv_obj_set_size(ui->screen_slider_2, 160, 9);

	//Write style state: LV_STATE_DEFAULT for style_screen_slider_2_main_main_default
	static lv_style_t style_screen_slider_2_main_main_default;
	lv_style_reset(&style_screen_slider_2_main_main_default);
	lv_style_set_radius(&style_screen_slider_2_main_main_default, 50);
	lv_style_set_bg_color(&style_screen_slider_2_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_slider_2_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_slider_2_main_main_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_slider_2_main_main_default, 100);
	lv_style_set_outline_color(&style_screen_slider_2_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_outline_width(&style_screen_slider_2_main_main_default, 0);
	lv_style_set_outline_opa(&style_screen_slider_2_main_main_default, 255);
	lv_style_set_pad_left(&style_screen_slider_2_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_slider_2_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_slider_2_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_slider_2_main_main_default, 0);
	lv_obj_add_style(ui->screen_slider_2, &style_screen_slider_2_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_DEFAULT for style_screen_slider_2_main_indicator_default
	static lv_style_t style_screen_slider_2_main_indicator_default;
	lv_style_reset(&style_screen_slider_2_main_indicator_default);
	lv_style_set_radius(&style_screen_slider_2_main_indicator_default, 50);
	lv_style_set_bg_color(&style_screen_slider_2_main_indicator_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_slider_2_main_indicator_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_slider_2_main_indicator_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_slider_2_main_indicator_default, 255);
	lv_obj_add_style(ui->screen_slider_2, &style_screen_slider_2_main_indicator_default, LV_PART_INDICATOR|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_DEFAULT for style_screen_slider_2_main_knob_default
	static lv_style_t style_screen_slider_2_main_knob_default;
	lv_style_reset(&style_screen_slider_2_main_knob_default);
	lv_style_set_radius(&style_screen_slider_2_main_knob_default, 50);
	lv_style_set_bg_color(&style_screen_slider_2_main_knob_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_slider_2_main_knob_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_slider_2_main_knob_default, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_slider_2_main_knob_default, 255);
	lv_obj_add_style(ui->screen_slider_2, &style_screen_slider_2_main_knob_default, LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_slider_set_range(ui->screen_slider_2,0, 100);
	lv_slider_set_value(ui->screen_slider_2,50,false);

	//Init events for screen
	events_init_screen(ui);
}