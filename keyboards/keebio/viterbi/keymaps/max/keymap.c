#include "viterbi.h"
#include "action_layer.h"
#include "eeconfig.h"

extern keymap_config_t keymap_config;

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.
#define _DVORAK 0
#define _LOWER 1
#define _RAISE 2
#define _ADJUST 16

enum custom_keycodes {
  DVORAK = SAFE_RANGE,
  LOWER,
  RAISE,
  ADJUST,
};

// Fillers to make layering more clear
#define KC_ KC_TRNS
#define _______ KC_TRNS
#define XXXXXXX KC_NO
#define KC_AJST ADJUST
#define KC_LOWR LOWER
#define KC_RASE RAISE
#define KC_UTLS UTILS

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  [_DVORAK] = LAYOUT_kc(
  //,----+----+----+----+----+----+----.    |----+----+----+----+----+----+----.
     ESC ,GRV , 1  , 2  , 3  , 4  ,  5 ,     DEL  , 6 , 7  , 8  , 9  , 0  ,EQL,
  //|----+----+----+----+----+----+----|    |----+----+----+----+----+----+----|
     TAB ,QUOT,COMM,DOT , P  , Y  ,LBRC,     BSPC, F  , G  , C  , R  , L  ,SLSH,
  //|----+----+----+----+----+----+----|    |----+----+----+----+----+----+----|
     LSFT, A  , O  , E  , U  , I  ,RBRC,     BSPC, D  , H  , T  , N  , S  ,MINS,
  //|----+----+----+----+----+----+----|    |----+----+----+----+----+----+----|
     LSFT,SCLN, Q  , J  , K  , X  ,ENTER,   ENTER, B  , M  , W  , V  , Z  ,RSFT,
  //|----+----+----+----+----+----+----|    |----+----+----+----+----+----+----|
     LCTL,PGDN,PGUP,LOWR,RASE,LALT,SPC ,     SPC ,RALT,	   ,LBRC,RBRC,BSLS,RCTL
  //`----+----+----+----+----+----+----'    `----+----+----+----+----+----+----'
  ),

  [_LOWER] = LAYOUT_kc(
  //,----+----+----+----+----+----+----.    ,----+----+----+----+----+----+----.
         ,    , F1 , F2 , F3 , F4 , F5 ,      F6 , F7 , F8 , F9 ,F10 ,F11 ,F12 ,
  //|----+----+----+----+----+----+----|    |----+----+----+----+----+----+----|
     UNDS,    ,EXLM, AT ,HASH,DLR ,PERC,     CIRC,AMPR,ASTR,LBRC,RBRC,SLSH, EQL,
  //|----+----+----+----+----+----+----|    |----+----+----+----+----+----+----|
     LSFT,    , F1 , F2 , F3 , F4 , F5 ,      F6 ,EQL ,HOME,PGDN,PGUP,END ,MINS,
  //|----+----+----+----+----+----+----|    |----+----+----+----+----+----+----|
     LSFT,    , F7 , F8 , F9 ,F10 ,F11 ,     F12 ,    ,    ,LPRN,RPRN,	  ,BSLS,
  //|----+----+----+----+----+----+----|    |----+----+----+----+----+----+----|
     LCTL,    ,    ,    ,    ,    ,    ,         ,    ,MNXT,VOLD,    ,	  ,RCTL
  //`----+----+----+----+----+----+----'    `----+----+----+----+----+----+----'
  ),

  [_RAISE] = LAYOUT_kc(
  //,----+----+----+----+----+----+----.    ,----+----+----+----+----+----+----.
         ,TILD, F1 , F2 , F3 , F4 , F5 ,      F6 , F7 , F8 , F9 ,F10 ,F11 ,F12 ,
  //|----+----+----+----+----+----+----|    |----+----+----+----+----+----+----|
     UNDS,    , 1  , 2  , 3  , 4  , 5  ,     HOME,END , 8  ,LBRC,RBRC,SLSH,EQL ,
  //|----+----+----+----+----+----+----|    |----+----+----+----+----+----+----|
     LSFT,    , F1 , F2 , F3 , F4 , F5 ,      F6 ,EQL ,LEFT,DOWN, UP ,RIGHT,MINS,
  //|----+----+----+----+----+----+----|    |----+----+----+----+----+----+----|
     LSFT,    , F7 , F8 , F9 ,F10 ,F11 ,     F12 ,NUHS,NUBS,LPRN,RPRN,    ,BSLS,
  //|----+----+----+----+----+----+----|    |----+----+----+----+----+----+----|
     LCTL,    ,    ,	,    ,    ,    ,         ,    ,MNXT,VOLD,    ,	  ,RCTL
  //`----+----+----+----+----+----+----'    `----+----+----+----+----+----+----'
  ),

  [_ADJUST] = LAYOUT(
    _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  _______, \
    _______, _______, RESET  , RGB_TOG, RGB_MOD, RGB_HUD, RGB_HUI, RGB_SAD, RGB_SAI, RGB_VAD, RGB_VAI, _______, KC_DEL,  _______, \
    _______, _______, _______, _______, AU_ON,   AU_OFF,  AG_NORM, AG_SWAP, DVORAK,  _______, _______, _______, _______, _______, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______ \
  )

};

#ifdef AUDIO_ENABLE
float tone_qwerty[][2]     = SONG(QWERTY_SOUND);
#endif

void persistent_default_layer_set(uint16_t default_layer) {
  eeconfig_update_default_layer(default_layer);
  default_layer_set(default_layer);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case DVORAK:
      if (record->event.pressed) {
        #ifdef AUDIO_ENABLE
          PLAY_NOTE_ARRAY(tone_qwerty, false, 0);
        #endif
        persistent_default_layer_set(1UL<<_DVORAK);
      }
      return false;
      break;
    case LOWER:
      if (record->event.pressed) {
        layer_on(_LOWER);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      } else {
        layer_off(_LOWER);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      }
      return false;
      break;
    case RAISE:
      if (record->event.pressed) {
        layer_on(_RAISE);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      } else {
        layer_off(_RAISE);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      }
      return false;
      break;
    case ADJUST:
      if (record->event.pressed) {
        layer_on(_ADJUST);
      } else {
        layer_off(_ADJUST);
      }
      return false;
      break;
  }
  return true;
}
