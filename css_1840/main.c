#include <main.h>
/*
 Addresses in PCF8574:
 A2A1A0    I2C address
 000   -   0x40
 001   -   0x42
 010   -   0x44
 011   -   0x46
 100   -   0x48
 101   -   0x4A
 110   -   0x4C
*/

byte addresses[7] = {0x40, 0x42, 0x44, 0x46, 0x48, 0x4A, 0x4C};

// :
byte dots_rows[5] = {0b11111111, 0b11111111, 0b11011011, 0b11111111,
                     0b11111111};

byte numbers[11][5] = {
    {// 0
     0b11000001, 0b10101110, 0b10110110, 0b10111010, 0b11000001},
    {// 1
     0b11111111, 0b10111101, 0b10000000, 0b10111111, 0b11111111},
    {// 2
     0b10111101, 0b10011110, 0b10101110, 0b10110110, 0b10111001},
    {// 3
     0b11011110, 0b10111110, 0b10111010, 0b10110100, 0b11001110},
    {// 4
     0b11100111, 0b11101011, 0b11101101, 0b10000000, 0b11101111},
    {// 5
     0b11011000, 0b10111010, 0b10111010, 0b10111010, 0b11000010},
    {// 6
     0b11000011, 0b10110101, 0b10110110, 0b10110110, 0b11001111},
    {// 7
     0b10011110, 0b11101110, 0b11110110, 0b11111010, 0b11111100},
    {// 8
     0b11001001, 0b10110110, 0b10110110, 0b10110110, 0b11001001},
    {// 9
     0b11111001, 0b10110110, 0b10110110, 0b11010110, 0b11100001},
    {// :
     0b11111111, 0b11111111, 0b11011011, 0b11111111, 0b11111111}};

byte indicators[12][5] = {0b11111111};

int16 input_arr[12] = {0};

void expander_write(byte i2c_addr, byte value) {
  i2c_start();
  i2c_write(i2c_addr);
  i2c_write(value);
  i2c_stop();
}

void write_expanders() {
  byte top_expander_value = 0b00000001;
  for (int8 i = 0; i < 5; ++i) {
    delay_ms(4);
    expander_write(addresses[0], ~top_expander_value);
    for (int8 j = 0; j < 6; ++j) {
      expander_write(addresses[j + 1], indicators[j][4 - i]);
    }
    top_expander_value <<= 1;
  }
}

void set_indicator(int8 ind_n, byte *ind_value) {
  for (int8 i = 0; i < 5; ++i) {
    indicators[ind_n][i] = ind_value[i];
  }
}

void watch() {
  // Minutes from 6 because 16:00 is initial time
  for (int16 minute = input_arr[2]; minute <= 9; ++minute) {
    set_indicator(2, numbers[minute]);
    write_expanders();
    for (int16 second1 = input_arr[4]; second1 <= 6; ++second1) {
      set_indicator(4, numbers[second1]);
      write_expanders();
      for (int16 second2 = input_arr[5]; second2 <= 9; ++second2) {
        set_indicator(5, numbers[second2]);
        write_expanders();
        write_expanders();
        write_expanders();
      }
    }
  }
}

void sycle_shift_line() {
  byte first_column = indicators[0][0];
  for (int8 i = 0; i <= 3; ++i) {
    indicators[0][i] = indicators[0][i + 1];
  }
  indicators[0][4] = indicators[1][0];

  for (int8 i = 0; i <= 3; ++i) {
    indicators[1][i] = indicators[1][i + 1];
  }
  indicators[1][4] = indicators[2][0];

  for (int8 i = 0; i <= 3; ++i) {
    indicators[2][i] = indicators[2][i + 1];
  }
  indicators[2][4] = indicators[3][0];

  for (int8 i = 0; i <= 3; ++i) {
    indicators[3][i] = indicators[3][i + 1];
  }
  indicators[3][4] = indicators[4][0];

  for (int8 i = 0; i <= 3; ++i) {
    indicators[4][i] = indicators[4][i + 1];
  }
  indicators[4][4] = indicators[5][0];

  for (int8 i = 0; i <= 3; ++i) {
    indicators[5][i] = indicators[5][i + 1];
  }
  indicators[5][4] = indicators[6][0];

  for (int8 i = 0; i <= 3; ++i) {
    indicators[6][i] = indicators[6][i + 1];
  }
  indicators[6][4] = indicators[7][0];

  for (int8 i = 0; i <= 3; ++i) {
    indicators[7][i] = indicators[7][i + 1];
  }
  indicators[7][4] = indicators[8][0];

  for (int8 i = 0; i <= 3; ++i) {
    indicators[8][i] = indicators[8][i + 1];
  }
  indicators[8][4] = indicators[9][0];

  for (int8 i = 0; i <= 3; ++i) {
    indicators[9][i] = indicators[9][i + 1];
  }
  indicators[9][4] = indicators[10][0];

  for (int8 i = 0; i <= 3; ++i) {
    indicators[10][i] = indicators[10][i + 1];
  }
  indicators[10][4] = indicators[11][0];

  for (int8 i = 0; i <= 3; ++i) {
    indicators[11][i] = indicators[11][i + 1];
  }

  indicators[11][4] = first_column;
}

int16 sym_to_number_index(char sym) {
  switch (sym) {
  case '0':
    return 0;
  case '1':
    return 1;
  case '2':
    return 2;
  case '3':
    return 3;
  case '4':
    return 4;
  case '5':
    return 5;
  case '6':
    return 6;
  case '7':
    return 7;
  case '8':
    return 8;
  case '9':
    return 9;
  case ':':
    return 10;
  }
}

void main() {
  puts("LED-Matrix-controller 5x7\n\rModes:");
  puts("1 - running line");
  puts("2 - flashing");
  puts("3 - watch");
  puts("Input: ");

  char mode_number = 0;
  while (TRUE) {
    mode_number = getc();
    if (mode_number == '1') {
      puts("\n\rInput line:");
      for (int8 i = 0; i < 12; ++i) {
        input_arr[i] = sym_to_number_index(getc());
      }
      puts("  - Running line mode processing!");

      // initial value
      set_indicator(0, numbers[input_arr[0]]);
      set_indicator(1, numbers[input_arr[1]]);
      set_indicator(2, numbers[input_arr[2]]);
      set_indicator(3, numbers[input_arr[3]]);
      set_indicator(4, numbers[input_arr[4]]);
      set_indicator(5, numbers[input_arr[5]]);
      set_indicator(6, numbers[input_arr[6]]);
      set_indicator(7, numbers[input_arr[7]]);
      set_indicator(8, numbers[input_arr[8]]);
      set_indicator(9, numbers[input_arr[9]]);
      set_indicator(10, numbers[input_arr[10]]);
      set_indicator(11, numbers[input_arr[11]]);
      while (TRUE) {
        write_expanders();
        write_expanders();
        sycle_shift_line();
      }
    } else if (mode_number == '2') {
      puts("\n\rInput 6 symbols:");
      for (int8 i = 0; i < 6; ++i) {
        input_arr[i] = sym_to_number_index(getc());
      }
      puts("  - Flashing mode processing!");
      set_indicator(0, numbers[input_arr[0]]);
      set_indicator(1, numbers[input_arr[1]]);
      set_indicator(2, numbers[input_arr[2]]);
      set_indicator(3, numbers[input_arr[3]]);
      set_indicator(4, numbers[input_arr[4]]);
      set_indicator(5, numbers[input_arr[5]]);
      while (TRUE) {
        write_expanders();
        write_expanders();
        for (int8 k = 0; k < 6; ++k) {
          // shutting down matrix
          expander_write(addresses[k], 0b11111111);
        }
        delay_ms(500);
      }
    } else if (mode_number == '3') {
      puts("\n\rInput 6 symbols:");
      for (int8 i = 0; i < 6; ++i) {
        input_arr[i] = sym_to_number_index(getc());
      }
      puts("  - Watch processing!");
      // Start as 16:00 (mm:ss).
      set_indicator(0, numbers[input_arr[0]]); // :
      set_indicator(1, numbers[input_arr[1]]); // m1
      set_indicator(2, numbers[input_arr[2]]); // m2
      set_indicator(3, numbers[input_arr[3]]); // :
      set_indicator(4, numbers[input_arr[4]]); // s1
      set_indicator(5, numbers[input_arr[5]]); // s2
      while (TRUE) {
        watch();
      }
    } else {
      puts("ERROR! Input mode from 1 to 3. Try again:");
    }
  }
}
