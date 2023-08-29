/*
 * Copyright 2023 NXP
 * SPDX-License-Identifier: MIT
 */

#include "events_init.h"
#include <stdio.h>
#include <stdlib.h>
#include "lvgl.h"

lv_obj_t* ddlist1;
extern uint16_t start_flag1;
extern uint16_t start_flag2;
extern uint16_t start_flag3;
extern uint16_t set_phase;

void events_init(lv_ui *ui)
{
}

static void screen_btn_1_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_CLICKED:
	{
        if(start_flag1 == 0)
        {
            if(start_flag2 == 0 && start_flag3 == 0)
            {
                start_flag1 = 1;
                lv_label_set_text(guider_ui.screen_btn_1_label, "STOP");
            }
        }
        else if (start_flag1 == 1)
        {
            start_flag1 = 0;
            lv_label_set_text(guider_ui.screen_btn_1_label, "START");
        }
	}
		break;
	default:
		break;
	}
}

static void screen_btn_2_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_CLICKED:
	{
        if(start_flag2 == 0)
        {
            if(start_flag1 == 0 && start_flag3 == 0)
            {
                start_flag2 = 1;
                lv_label_set_text(guider_ui.screen_btn_2_label, "OFF");
            }
        }
        else if (start_flag2 == 1)
        {
            start_flag2 = 0;
            lv_label_set_text(guider_ui.screen_btn_2_label, "ON");
        }
	}
		break;
	default:
		break;
	}
}

static void screen_btn_3_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_CLICKED:
	{
        if(start_flag3 == 0)
        {
            if(start_flag1 == 0 && start_flag2 == 0)
            {
                start_flag3 = 1;
                lv_label_set_text(guider_ui.screen_btn_3_label, "OFF");
            }
        }
        else if (start_flag3 == 1)
        {
            start_flag3 = 0;
            lv_label_set_text(guider_ui.screen_btn_3_label, "ON");
        }
	}
		break;
	default:
		break;
	}
}

static void screen_ddlist_1_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_VALUE_CHANGED:
	{
        char buf[32];
        lv_dropdown_get_selected_str(ddlist1, buf, sizeof(buf));
        set_phase = atoi(buf);
	}
		break;
	default:
		break;
	}
}

static void screen_slider_1_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_VALUE_CHANGED:
	{
	}
		break;
	default:
		break;
	}
}

void events_init_screen(lv_ui *ui)
{
    ddlist1 = ui->screen_ddlist_1;
	lv_obj_add_event_cb(ui->screen_btn_1, screen_btn_1_event_handler, LV_EVENT_ALL, NULL);
	lv_obj_add_event_cb(ui->screen_btn_2, screen_btn_2_event_handler, LV_EVENT_ALL, NULL);
	lv_obj_add_event_cb(ui->screen_btn_3, screen_btn_3_event_handler, LV_EVENT_ALL, NULL);
	lv_obj_add_event_cb(ui->screen_ddlist_1, screen_ddlist_1_event_handler, LV_EVENT_ALL, NULL);
	lv_obj_add_event_cb(ui->screen_slider_1, screen_slider_1_event_handler, LV_EVENT_ALL, NULL);
}
