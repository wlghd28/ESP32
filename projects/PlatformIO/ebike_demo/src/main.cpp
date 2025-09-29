#include <Arduino.h>
#include <lvgl.h>
#include <ui.h>
#include <Arduino_GFX_Library.h>
#define TFT_BL 2
//#define GFX_BL DF_GFX_BL

Arduino_ESP32RGBPanel *bus = new Arduino_ESP32RGBPanel(
    GFX_NOT_DEFINED /* CS */, GFX_NOT_DEFINED /* SCK */, GFX_NOT_DEFINED /* SDA */,
    41 /* DE */, 40 /* VSYNC */, 39 /* HSYNC */, 42 /* PCLK */,
    14 /* R0 */, 21 /* R1 */, 47 /* R2 */, 48 /* R3 */, 45 /* R4 */,
    9 /* G0 */, 46 /* G1 */, 3 /* G2 */, 8 /* G3 */, 16 /* G4 */, 1 /* G5 */,
    15 /* B0 */, 7 /* B1 */, 6 /* B2 */, 5 /* B3 */, 4 /* B4 */
);

Arduino_RPi_DPI_RGBPanel *gfx = new Arduino_RPi_DPI_RGBPanel(
  bus,
//  800 /* width */, 0 /* hsync_polarity */, 8/* hsync_front_porch */, 2 /* hsync_pulse_width */, 43/* hsync_back_porch */,
//  480 /* height */, 0 /* vsync_polarity */, 8 /* vsync_front_porch */, 2/* vsync_pulse_width */, 12 /* vsync_back_porch */,
//  1 /* pclk_active_neg */, 16000000 /* prefer_speed */, true /* auto_flush */);

    800 /* width */, 0 /* hsync_polarity */, 210 /* hsync_front_porch */, 30 /* hsync_pulse_width */, 16 /* hsync_back_porch */,
    480 /* height */, 0 /* vsync_polarity */, 22 /* vsync_front_porch */, 13 /* vsync_pulse_width */, 10 /* vsync_back_porch */,
    1 /* pclk_active_neg */, 12000000 /* prefer_speed */, true /* auto_flush */);

#include "touch.h"

// custom
// header
#include "controller.h"
#include "custom_ui.h"
#include "custom_task.h"

// define
#define LCD_W 800
#define LCD_H 480
#define RX_CONTROLLER 18
#define TX_CONTROLLER 17

// global var
Controller *g_controller;
Custom_UI *g_custom_ui;
Custom_Task *g_custom_task;
uint16_t g_timer_count_ui;
//

/*Change to your screen resolution*/
static const uint16_t screenWidth  = 800;
static const uint16_t screenHeight = 480;
static lv_color_t *disp_draw_buf;

enum { SCREENBUFFER_SIZE_PIXELS = screenWidth * screenHeight / 10 };
static lv_color_t buf [SCREENBUFFER_SIZE_PIXELS];

#if LV_USE_LOG != 0
/* Serial debugging */
void my_print(const char * buf)
{
    Serial.printf(buf);
    Serial.flush();
}
#endif


/* Display flushing */
void my_disp_flush (lv_display_t *disp, const lv_area_t *area, uint8_t *pixelmap)
{
  //   uint32_t w = (area->x2 - area->x1 + 1);
  //   uint32_t h = (area->y2 - area->y1 + 1);

  // #if (LV_COLOR_16_SWAP != 0)
  //   gfx->draw16bitBeRGBBitmap(area->x1, area->y1, (uint16_t *)pixelmap, w, h);
  // #else
  //   gfx->draw16bitRGBBitmap(area->x1, area->y1, (uint16_t *)pixelmap, w, h);
  // #endif

  //  lv_disp_flush_ready(disp);

    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    static uint16_t line_buf[LCD_W];

    // 전체 화면 폭 (LVGL 버퍼 기준)
    uint32_t screen_w = screenWidth; // 800

    for (int16_t row = 0; row < h; row++)
    {
        // 원본 줄 시작 포인터 계산
        uint16_t *src_line = ((uint16_t *)pixelmap) + row * w;  

        // 좌우 반전
        for (int16_t col = 0; col < w; col++)
        {
            line_buf[col] = src_line[w - 1 - col];
        }

        // 회전된 좌표 계산
        int16_t rotated_x = LCD_W - area->x2 - 1;
        int16_t rotated_y = LCD_H - (area->y1 + row) - 1;

#if (LV_COLOR_16_SWAP != 0)
        gfx->draw16bitBeRGBBitmap(rotated_x, rotated_y, line_buf, w, 1);
#else
        gfx->draw16bitRGBBitmap(rotated_x, rotated_y, line_buf, w, 1);
#endif
    }

    lv_disp_flush_ready(disp);
}

/*Read the touchpad*/
void my_touchpad_read(lv_indev_t * indev_driver, lv_indev_data_t * data)
{
    if (touch_has_signal())
    {
      if (touch_touched())
      {
        data->state = LV_INDEV_STATE_PR;

        /*Set the coordinates*/
        data->point.x = touch_last_x;
        data->point.y = touch_last_y;
        //Serial.print( "Data x " );
        //Serial.println( data->point.x );
        //Serial.print( "Data y " );
        //Serial.println( data->point.y );
      }
      else if (touch_released())
      {
        data->state = LV_INDEV_STATE_REL;
      }
    }
    else
    {
      data->state = LV_INDEV_STATE_REL;
    }
}

/*Set tick routine needed for LVGL internal timings*/
static uint32_t my_tick_get_cb (void) { return millis(); }

void setup()
{
    Serial.begin(115200); /* prepare for possible serial debug */
    Serial1.begin(115200, SERIAL_8N1, RX_CONTROLLER, TX_CONTROLLER);  /* communication to controller */

    // String LVGL_Arduino = "Hello Arduino! ";
    // LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();

    // Serial.println( LVGL_Arduino );
    // Serial.println( "I am LVGL_Arduino" );

#if LV_USE_LOG != 0
    lv_log_register_print_cb( my_print ); /* register print function for debugging */
#endif

    gfx->begin();
#ifdef TFT_BL
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);
    ledcSetup(0, 300, 8);
    ledcAttachPin(TFT_BL, 0);
    ledcWrite(0, 0); /* Screen brightness can be modified by adjusting this parameter. (0-255) */
#endif

    lv_init();

    // Init touch device
    pinMode(TOUCH_GT911_RST, OUTPUT);
    digitalWrite(TOUCH_GT911_RST, LOW);
    delay(10);
    digitalWrite(TOUCH_GT911_RST, HIGH);
    delay(10);
    touch_init();
    //  touch.setTouch( calData );

    static lv_disp_t* disp;
    disp = lv_display_create( screenWidth, screenHeight );
    lv_display_set_buffers( disp, buf, NULL, SCREENBUFFER_SIZE_PIXELS * sizeof(lv_color_t), LV_DISPLAY_RENDER_MODE_PARTIAL );
    lv_display_set_flush_cb( disp, my_disp_flush );
    lv_display_set_rotation( disp, LV_DISPLAY_ROTATION_180 );

    static lv_indev_t* indev;
    indev = lv_indev_create();
    lv_indev_set_type( indev, LV_INDEV_TYPE_POINTER );
    lv_indev_set_read_cb( indev, my_touchpad_read );

    lv_tick_set_cb( my_tick_get_cb );

    ui_init();

    // custom
    g_controller = new Controller();
    g_custom_ui = new Custom_UI(g_controller);
    g_custom_task = new Custom_Task(g_custom_ui, g_controller);
    //

    Serial.println( "Setup done" );
}

void loop()
{
    lv_timer_handler(); /* let the GUI do its work */
    if(++g_timer_count_ui >= 200)
    {
        g_custom_ui->DisplayData();
        g_timer_count_ui = 0;
    }

    delay(5);
}



