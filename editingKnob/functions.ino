//ENCODER KNOB FUNCTIONS
void turnAction(bool directionTurning) {
  switch (state) {
    case 0: //shift
      if (directionTurning)TrinketHidCombo.pressKey(KEYCODE_MOD_LEFT_SHIFT, KEYCODE_ARROW_LEFT);
      else TrinketHidCombo.pressKey(KEYCODE_MOD_LEFT_SHIFT, KEYCODE_ARROW_RIGHT);
      TrinketHidCombo.pressKey(0, 0);
      break;
    case 1: //media
      if (directionTurning)TrinketHidCombo.pressMultimediaKey(MMKEY_VOL_DOWN);
      else TrinketHidCombo.pressMultimediaKey(MMKEY_VOL_UP);
      break;
    case 2: //premiere pro
      if (directionTurning) TrinketHidCombo.pressKey(0, KEYCODE_ARROW_LEFT);
      else TrinketHidCombo.pressKey(0, KEYCODE_ARROW_RIGHT);
      TrinketHidCombo.pressKey(0, 0);
      break;
    case 3: //photoshop
      if (directionTurning)TrinketHidCombo.pressKey(0, KEYCODE_SQBRAK_LEFT);
      else TrinketHidCombo.pressKey(0, KEYCODE_SQBRAK_RIGHT);
      TrinketHidCombo.pressKey(0, 0);
    default:
      break;
  }
}

//CLICK FUNCTIONS
void clickAction(bool isDouble) { //true for doubleclick, false single click
  switch (state) {
    case 0: //shift

      TrinketHidCombo.pressKey(0, 0);
      break;
    case 1: //media
      if (isDouble)TrinketHidCombo.pressMultimediaKey(MMKEY_SCAN_NEXT_TRACK);
      else TrinketHidCombo.pressMultimediaKey(MMKEY_PLAYPAUSE);
      break;
    case 2: //arrow
      if (isDouble) TrinketHidCombo.pressKey(KEYCODE_MOD_LEFT_CONTROL, KEYCODE_K);
      else TrinketHidCombo.pressKey(0, KEYCODE_SPACE);
      TrinketHidCombo.pressKey(0, 0);
      break;
    case 3: //photoshop

      TrinketHidCombo.pressKey(0, 0);
    default:
      break;
  }
}
