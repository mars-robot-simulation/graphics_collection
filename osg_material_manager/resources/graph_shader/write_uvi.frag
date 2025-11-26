void write_uvi(vec2 texCoord, float intensity, out vec4 color) {
  float r, g, b, a;
  float s = 1.0/255.0;
  float x = texCoord.x;
  float y = texCoord.y;
  float gScale = 0.003891045;
  float gScaleT = 1.0/gScale;
  float rScale = 1-gScale;
  float rScaleT = 1.0/rScale;
  g = mod(x, gScale);
  r = (x - g)*rScaleT;
  g = g*gScaleT;
  a = mod(y, gScale);
  b = (y - a)*rScaleT;
  a = a*gScaleT;
  if(intensity > 0.1)
      color = vec4(r, g, b, a);
  else
      color = vec4(0.0, 0.0, 0.0, 1.0);
}
