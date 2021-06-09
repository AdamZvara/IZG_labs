#ifdef VERTEX_SHADER

uniform mat4 view;
uniform mat4 proj;
#line 5

out highp vec2 vCoord;
flat out int matrialId;

void drawQuad(int vId,vec2 size,vec2 offset,mat4 mvp,int material){
  if(vId==0){
    matrialId = material;
    vCoord = vec2(0.f,0.f);
    gl_Position = mvp*vec4(vec2(-1.f,-1.f)*size+offset,0.f,1.f);
  }
  if(vId==1){
    matrialId = material;
    vCoord = vec2(1.f,0.f);
    gl_Position = mvp*vec4(vec2(+1.f,-1.f)*size+offset,0.f,1.f);
  }
  if(vId==2){
    matrialId = material;
    vCoord = vec2(0.f,1.f);
    gl_Position = mvp*vec4(vec2(-1.f,+1.f)*size+offset,0.f,1.f);
  }
  if(vId==3){
    matrialId = material;
    vCoord = vec2(0.f,1.f);
    gl_Position = mvp*vec4(vec2(-1.f,+1.f)*size+offset,0.f,1.f);
  }
  if(vId==4){
    matrialId = material;
    vCoord = vec2(1.f,0.f);
    gl_Position = mvp*vec4(vec2(+1.f,-1.f)*size+offset,0.f,1.f);
  }
  if(vId==5){
    matrialId = material;
    vCoord = vec2(1.f,1.f);
    gl_Position = mvp*vec4(vec2(+1.f,+1.f)*size+offset,0.f,1.f);
  }
}

void main(){
  if(gl_VertexID>=12){
    gl_Position = vec4(0.f,0.f,0.f,1.f);
    return;
  }

  mat4 mvp = proj*view;

  drawQuad(gl_VertexID  ,vec2(1.f,2.f/3.f),vec2(-2.f,0.f),mvp,0);

  drawQuad(gl_VertexID-6,vec2(1.f,2.f/3.f),vec2( 2.f,0.f),mvp,1);


}
#endif



#ifdef FRAGMENT_SHADER
flat in int matrialId;
in highp vec2 vCoord;
out highp vec4 fColor;

highp vec4 japan(highp vec2 uv){
  uv *= vec2(3,2);
  highp float dist = distance(uv,vec2(1.5,1));
  highp vec3 white = vec3(224,224,224)/vec3(255);
  highp vec3 red   = vec3(176,0  ,15 )/vec3(255); 
  return mix(vec4(white,1),vec4(red,1),float(dist<3./5.));
}

highp vec4 thailand(highp vec2 uv){
  uv *= vec2(9.f,6.f);
  highp vec3 white = vec3(0xf4,0xf5,0xf8)/vec3(255);
  highp vec3 red   = vec3(0xa5,0x19,0x31)/vec3(255); 
  highp vec3 blue  = vec3(0x2d,0x2a,0x4a)/vec3(255);
  highp float dist = abs(uv.y-3.f);
  if(dist<1.f)return vec4(blue,1.f);
  if(dist<2.f)return vec4(white,1.f);
  return vec4(red,1.f);
}

void main(){

  if(matrialId==0)fColor = japan   (vCoord);
  if(matrialId==1)fColor = thailand(vCoord);
}
#endif
