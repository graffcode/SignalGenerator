void TFT_ColumnPage(Uint x1, Uint x2, Uint y1, Uint y2)
{      
  TFT_CS = 0;
  TFT_DC = 0; SPI_Write(0x2A);
  TFT_DC = 1; SPI_Write(x1 >> 8); SPI_Write(x1 & 0xFF); SPI_Write(x2 >> 8); SPI_Write(x2 & 0xFF);
  TFT_DC = 0; SPI_Write(0x2B);
  TFT_DC = 1; SPI_Write(y1 >> 8); SPI_Write(y1 & 0xFF); SPI_Write(y2 >> 8); SPI_Write(y2 & 0xFF);
  TFT_DC = 0; SPI_Write(0x2C);
  TFT_CS = 1;
}

Uint TFT_RGBConvert(Uint red, Uint green, Uint blue)
{
  Uint color = 0;
  red = (red & 0xF8) << 8; 
  green = (green & 0xFC) << 3; 
  blue = (blue & 0xF8) >> 3;
  color = red | green | blue;
  return color;
}

void TFT_Pixel(Uint x, Uint y, Uint color)
{
  TFT_CS = 0;
  TFT_DC = 0; SPI_Write(0x2A);
  TFT_DC = 1; SPI_Write(x >> 8); SPI_Write(x & 0xFF); SPI_Write(x >> 8); SPI_Write(x & 0xFF);
  TFT_DC = 0; SPI_Write(0x2B);
  TFT_DC = 1; SPI_Write(y >> 8); SPI_Write(y & 0xFF); SPI_Write(y >> 8); SPI_Write(y & 0xFF);
  TFT_DC = 0; SPI_Write(0x2C); 
  TFT_DC = 1; SPI_Write(color >> 8); SPI_Write(color & 0xFF);
  TFT_CS = 1;
}

void TFT_SetDotSize(Uchar size_)
{
  dot_size = size_;
}

void TFT_Dot(Uint x, Uint y, Uint color)
{
  Uchar i;
  switch(dot_size)
      {
       case 0:  i = 1;   break;
       case 1:  i = 9;   break;
       case 2:  i = 25;  break;
       case 3:  i = 49;  break;
       case 4:  i = 81;  break;
       case 5:  i = 121; break;
       default: i = 121; break;
      }
  TFT_ColumnPage(x - dot_size, x + dot_size, y - dot_size, y + dot_size);
  TFT_CS = 0; TFT_DC = 1;
  while(i)
  {
    SPI_Write(color >> 8);
    SPI_Write(color & 0xFF);      
    i--;
  }
  TFT_CS = 1;
}

void TFT_SetFont(CUchar *font_, Uchar letterspacing)
{
  font2 = font_;
  letter_spacing = letterspacing;
  height = TFT_CharHeight();
}

Uchar TFT_CharHeight(void)
{
  font = font2;
  font += 6;
  return *font;
}

Uint TFT_TextWidth(Schar *buffer)
{
  Uint p, text_width = 0;
  while(*buffer)
  {
    p = *buffer - 32; p = p * 4; p = p + 8;       
    font = font2;                
    font += p;                                   
    text_width += *font;
    text_width += letter_spacing;
    buffer++;
  }    
  return text_width;
}

void TFT_WriteChar(Uchar c, Uint x, Uint y, Uint color1, Uint color2)
{
  Uchar i, j, k;
  Uint p;
  p = c - 32; p = p * 4; p = p + 8;       
  font = font2;                
  font += p;                  
  width = *font;
  font += 2; p = *font;
  font -= 1; p = (p << 8) | *font;
  font = font2;
  font += p;
  i = height;
  TFT_ColumnPage(x, x + width + letter_spacing - 1, y, y + height);
  TFT_CS = 0; TFT_DC = 1; 
  while(i)
  {
    j = width;
    while(j)
    {
      if(j>0){if(*font&0x01){SPI_Write(color1>>8);SPI_Write(color1&0xFF);}else{SPI_Write(color2>>8);SPI_Write(color2&0xFF);}j--;}else{font++;break;}
      if(j>0){if(*font&0x02){SPI_Write(color1>>8);SPI_Write(color1&0xFF);}else{SPI_Write(color2>>8);SPI_Write(color2&0xFF);}j--;}else{font++;break;}
      if(j>0){if(*font&0x04){SPI_Write(color1>>8);SPI_Write(color1&0xFF);}else{SPI_Write(color2>>8);SPI_Write(color2&0xFF);}j--;}else{font++;break;}
      if(j>0){if(*font&0x08){SPI_Write(color1>>8);SPI_Write(color1&0xFF);}else{SPI_Write(color2>>8);SPI_Write(color2&0xFF);}j--;}else{font++;break;}
      if(j>0){if(*font&0x10){SPI_Write(color1>>8);SPI_Write(color1&0xFF);}else{SPI_Write(color2>>8);SPI_Write(color2&0xFF);}j--;}else{font++;break;}
      if(j>0){if(*font&0x20){SPI_Write(color1>>8);SPI_Write(color1&0xFF);}else{SPI_Write(color2>>8);SPI_Write(color2&0xFF);}j--;}else{font++;break;}
      if(j>0){if(*font&0x40){SPI_Write(color1>>8);SPI_Write(color1&0xFF);}else{SPI_Write(color2>>8);SPI_Write(color2&0xFF);}j--;}else{font++;break;}
      if(j>0){if(*font&0x80){SPI_Write(color1>>8);SPI_Write(color1&0xFF);}else{SPI_Write(color2>>8);SPI_Write(color2&0xFF);}j--;}else{font++;break;}
      font++;
    }
  for(k = 0; k < letter_spacing; k++){SPI_Write(color2 >> 8); SPI_Write(color2 & 0xFF);}
  i--;
  }
  TFT_CS = 1;
}

void TFT_ConstText(CSchar *buffer, Uint x, Uint y, Uint color1, Uint color2)
{
  while(*buffer)                
  {
    TFT_WriteChar(*buffer, x, y, color1, color2);
    x += width + letter_spacing;
    buffer++;               
  } 
}

void TFT_Text(Schar *buffer, Uint x, Uint y, Uint color1, Uint color2)
{
while(*buffer)                
  {
    TFT_WriteChar(*buffer, x, y, color1, color2);
    x += width + letter_spacing;
    buffer++;               
  } 
}

void TFT_TextLEFT(Schar *buffer, Uint y, Uint color1, Uint color2){
Uint x = 0;
while(*buffer)                
     {
      TFT_WriteChar(*buffer, x, y, color1, color2);
      x += width + letter_spacing;
      buffer++;               
     }
TFT_Box(x, y, tft_x, y + height - 1, color2);
}

void TFT_TextCENTER(Schar *buffer, Uint y, Uint color1, Uint color2)
{
  Uint x;
  x = TFT_TextWidth(buffer);
  x = tft_x - x;
  x = x / 2;
  TFT_Box(0, y, x, y + height - 1, color2);
  x++;
  while(*buffer)                
    {
      TFT_WriteChar(*buffer, x, y, color1, color2);
      x += width + letter_spacing;
      buffer++;               
    }
  TFT_Box(x, y, tft_x, y + height - 1, color2);
}

void TFT_TextRIGHT(Schar *buffer, Uint y, Uint color1, Uint color2)
{
  Uint x;
  x = TFT_TextWidth(buffer);
  x = tft_x - x;
  TFT_Box(0, y, x, y + height - 1, color2);
  x++;
  while(*buffer)                
     {
        TFT_WriteChar(*buffer, x, y, color1, color2);
        x += width + letter_spacing;
        buffer++;               
     }
}


void TFT_FillScreen(Uint color)
{    
  Uchar DH, DL;
  Uint i, j;
  DH = color >> 8;
  DL = color & 0xFF;
  switch(frame_memory)
  {
    case TFT_VERTICAL:   TFT_ColumnPage(0, TFT_W - 1, 0, TFT_H - 1); break;
    case TFT_HORIZONTAL: TFT_ColumnPage(0, TFT_H - 1, 0, TFT_W - 1); break;    
  }
  TFT_CS = 0; TFT_DC = 1;
  for(i = 0; i < TFT_H; i++)
  {
    for (j = 0; j < TFT_W; j++)
    {
      SPI_Write(DH);
      SPI_Write(DL);
    }
  }
  TFT_CS = 1;
}

void TFT_Box(Uint x1, Uint y1, Uint x2, Uint y2, Uint color)
{
  Uchar DH, DL;
  Uint i, j;
  DH = color >> 8;
  DL = color & 0xFF;
  TFT_ColumnPage(x1, x2, y1, y2);
  TFT_CS = 0; TFT_DC = 1;
  for(i = y1; i <= y2; i++)
  {
    for (j = x1; j <= x2; j++)
    {
      SPI_Write(DH);
      SPI_Write(DL);        
    }
  }
  TFT_CS = 1;
}

void TFT_Line(Uint x1, Uint y1, Uint x2, Uint y2, Uint color)
{
  Sint i;
  Sint longest, shortest; 
  Sint numerator;
  Sint w = x2 - x1;
  Sint h = y2 - y1;
  Sint dx1 = 0, dy1 = 0, dx2 = 0, dy2 = 0;

  if(w < 0) dx1 = -1; else if(w > 0) dx1 = 1;
  if(h < 0) dy1 = -1; else if(h > 0) dy1 = 1;
  if(w < 0) dx2 = -1; else if(w > 0) dx2 = 1;
  if(w < 0) w *= -1;
  if(h < 0) h *= -1;
  longest = w; 
  shortest = h;
  if(!(longest > shortest)) 
  {//if(w < 0) w *= -1; //if(h < 0) h *= -1; 
    longest = h; 
    shortest = w;
    if(h < 0) dy2 = -1; else if(h > 0) dy2 = 1;
    dx2 = 0;         
  }
  numerator = longest >> 1;
  for (i = 0; i <= longest; i++)
  {
    TFT_Dot(x1, y1, color);
    numerator += shortest;
    if(!(numerator < longest)) 
    {
      numerator -= longest;
      x1 += dx1;
      y1 += dy1;
    }
    else
    {
      x1 += dx2;
      y1 += dy2;
    }
  }
}

void TFT_Rectangle(Uint x1, Uint y1, Uint x2, Uint y2, Uint color)
{
  TFT_Line(x1, y1, x2, y1, color);
  TFT_Line(x1, y2, x2, y2, color);
  TFT_Line(x1, y1, x1, y2, color);
  TFT_Line(x2, y1, x2, y2, color);
}

void TFT_RectangleRound(Uint x1, Uint y1, Uint x2, Uint y2, Uint radius, Uint color)
{
  TFT_Line(x1 + radius, y1, x2 - radius, y1, color);
  TFT_Line(x1 + radius, y2, x2 - radius, y2, color);
  TFT_Line(x1, y1 + radius, x1, y2 - radius, color);
  TFT_Line(x2, y1 + radius, x2, y2 - radius, color);
  TFT_RoundEdge(x1 + radius, y1 + radius, 3, radius, color); 
  TFT_RoundEdge(x2 - radius, y1 + radius, 4, radius, color); 
  TFT_RoundEdge(x1 + radius, y2 - radius, 2, radius, color); 
  TFT_RoundEdge(x2 - radius, y2 - radius, 1, radius, color); 
}

void TFT_RectangleRoundFill(Uint x1, Uint y1, Uint x2, Uint y2, Uint radius, Uint color)
{ 
  TFT_Box(x1 + radius, y1, x2 - radius, y2, color);
  TFT_Box(x1, y1 + radius, x2, y2 - radius, color);
  TFT_CircleFill(x1 + radius, y1 + radius, radius, color); 
  TFT_CircleFill(x2 - radius, y1 + radius, radius, color); 
  TFT_CircleFill(x1 + radius, y2 - radius, radius, color); 
  TFT_CircleFill(x2 - radius, y2 - radius, radius, color);
}

void TFT_Circle(Uint x1, Uint y1, Uint radius, Uint color)
{  
  Sint x = radius, y = 0;
  Sint radiusError = 1 - x;
  while(x >= y)
  {
    TFT_Dot(x + x1, y + y1, color);
    TFT_Dot(y + x1, x + y1, color);
    TFT_Dot(-x + x1, y + y1, color);
    TFT_Dot(-y + x1, x + y1, color);
    TFT_Dot(-x + x1, -y + y1, color);
    TFT_Dot(-y + x1, -x + y1, color);
    TFT_Dot(x + x1, -y + y1, color);
    TFT_Dot(y + x1, -x + y1, color);
    y++;
    if(radiusError<0)
    {
      radiusError += 2 * y + 1;
     }
    else
    {
      x--;
      radiusError += 2 * (y - x + 1);
    }
  }
}

void TFT_CircleFill(Uint x1, Uint y1, Uint radius, Uint color)
{
  Uchar i = dot_size;    
  Sint x = radius, y = 0;
  Sint radiusError = 1 - x;
  dot_size = 0; 
  while(x >= y)
  {
    TFT_Line(x1-x, y1+y, x1+x, y1+y, color);
    TFT_Line(x1-x, y1-y, x1+x, y1-y, color);
    TFT_Line(x1-y, y1+x, x1+y, y1+x, color);
    TFT_Line(x1-y, y1-x, x1+y, y1-x, color);
    TFT_Dot(x + x1, y + y1, color);
    TFT_Dot(y + x1, x + y1, color);
    TFT_Dot(-x + x1, y + y1, color);
    TFT_Dot(-y + x1, x + y1, color);
    TFT_Dot(-x + x1, -y + y1, color);
    TFT_Dot(-y + x1, -x + y1, color);
    TFT_Dot(x + x1, -y + y1, color);
    TFT_Dot(y + x1, -x + y1, color);
    y++;
    if(radiusError<0)
    {
      radiusError += 2 * y + 1;
    }
    else
    {
      x--;
      radiusError += 2 * (y - x + 1);
    }
  }
  dot_size = i;
}

void TFT_RoundEdge(Uint x1, Uint y1, Uint alignment, Uint radius, Uint color)
{
  Sint x = radius, y = 0;
  Sint radiusError = 1 - x;
  while(x >= y)
  {
    switch(alignment)
    {
      case 1: 
              TFT_Dot(x + x1, y + y1, color);
              TFT_Dot(y + x1, x + y1, color);
              break;
      case 2:
              TFT_Dot(-x + x1, y + y1, color);
              TFT_Dot(-y + x1, x + y1, color); 
              break;
      case 3:
              TFT_Dot(-x + x1, -y + y1, color);
              TFT_Dot(-y + x1, -x + y1, color); 
              break;
      case 4:
              TFT_Dot(x + x1, -y + y1, color);
              TFT_Dot(y + x1, -x + y1, color);
              break;
    }      
    y++;
    if(radiusError<0)
    {
      radiusError += 2 * y + 1;
    }
    else
    {
      x--;
      radiusError += 2 * (y - x + 1);
    }
  }
}

void TFT_Icon(CUint *buffer, Uint x, Uint y, Uchar width_, Uchar height_)
{
  Uint i, j;    
  TFT_ColumnPage(x, x + (width_ - 1), y, y + (height_ - 1));
  TFT_CS = 0; TFT_DC = 1;
  for(i = 0; i < height_; i++)
  {
    for(j = 0; j < width_; j++)
    {
      SPI_Write(*buffer >> 8);
      SPI_Write(*buffer & 0xFF);    
      buffer++;
    }
  }
  TFT_CS = 1;
}
