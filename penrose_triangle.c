#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <stdlib.h>

#define WIN_WIDTH_PX (640)
#define WIN_HEIGHT_PX (640)

#define FPS (60.0f)

static ALLEGRO_DISPLAY *display = NULL;
static ALLEGRO_EVENT_QUEUE *eq = NULL;
static ALLEGRO_TIMER *timer = NULL;
static bool redraw = true;
static bool doexit = false;

static void Initialize();
static void Terminate();
static void Draw();
static void ProcessInput(ALLEGRO_EVENT *ev);
static void Update(double dt, ALLEGRO_EVENT *ev);
static bool IsRunning();

int main() {
  double timeNow = 0.0, prevTime = 0.0, dt = 0.0;
  ALLEGRO_EVENT ev;

  Initialize();

  while (IsRunning()) {
    al_wait_for_event(eq, &ev);
    timeNow = al_get_time();
    dt = timeNow - prevTime;

    ProcessInput(&ev);
    Update(dt, &ev);
    Draw();

    prevTime = timeNow;
  }

  Terminate();
  return 0;
}

static void Initialize() {
  if (!al_init()) {
    fprintf(stderr, "ERROR: Failed to initialize allegro!\n");
    goto init_fail;
  }

  if (!al_init_primitives_addon()) {
    fprintf(stderr, "ERROR: Failed to load primitives addon!\n");
    goto prim_addon_fail;
  }

  timer = al_create_timer(1.0 / FPS);
  if (!timer) {
    fprintf(stderr, "ERROR: Failed to create timer!\n");
    goto timer_fail;
  }

  display = al_create_display(WIN_WIDTH_PX, WIN_HEIGHT_PX);
  if (!display) {
    fprintf(stderr, "ERROR: Failed to create display!\n");
    goto disp_fail;
  }

  eq = al_create_event_queue();
  if (!eq) {
    fprintf(stderr, "ERROR: Failed to create event queue!\n");
    goto eq_fail;
  }

  al_register_event_source(eq, al_get_display_event_source(display));
  al_register_event_source(eq, al_get_timer_event_source(timer));

  al_set_window_title(display, "Penrose Triangle");
  al_start_timer(timer);

  return;

eq_fail:
disp_fail:
  al_destroy_timer(timer);
timer_fail:
prim_addon_fail:
init_fail:
  exit(1);
}

static void Terminate() {
  al_destroy_event_queue(eq);
  al_destroy_display(display);
  al_destroy_timer(timer);
}

static void Draw() {
  if (redraw && al_is_event_queue_empty(eq)) {
    redraw = false;
    al_clear_to_color(al_map_rgb(183, 173, 160));

    float vr[] = {340, 9, 121, 392, 181, 392, 343, 113, 563, 498, 595, 443};
    float vg[] = {344, 113, 314, 164, 474, 443, 33, 443, 64, 498, 562, 498};
    float vb[] = {340, 9, 276, 9, 33, 443, 474, 443, 444, 392, 121, 392};

    al_draw_filled_polygon(vr, 6, al_map_rgb(255, 0, 0));
    al_draw_filled_polygon(vg, 6, al_map_rgb(0, 255, 0));
    al_draw_filled_polygon(vb, 6, al_map_rgb(0, 0, 255));

    al_flip_display();
  }
}

static void ProcessInput(ALLEGRO_EVENT *ev) {}

static void Update(double dt, ALLEGRO_EVENT *ev) {
  static double updateTimer = 0.0;
  if (ev->type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
    doexit = true;
    return;
  } else if (ev->type == ALLEGRO_EVENT_TIMER) {
    redraw = true;
  }
}

static bool IsRunning() { return !doexit; }
