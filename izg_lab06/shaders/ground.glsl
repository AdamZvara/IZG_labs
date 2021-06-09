#ifdef VERTEX_SHADER

uniform mat4 view;
uniform mat4 proj;
#line 5

out highp vec2 vCoord;
void main(){
  if(gl_VertexID>=6){
    gl_Position = vec4(0.f,0.f,0.f,1.f);
    return;
  }

  mat4 mvp = proj*view;

  if(gl_VertexID==0){
    vCoord = vec2(0.f,0.f);
    gl_Position = mvp*vec4(-1.f,0.f,-1.f,1.f);
  }
  if(gl_VertexID==1){
    vCoord = vec2(1.f,0.f);
    gl_Position = mvp*vec4(+1.f,0.f,-1.f,1.f);
  }
  if(gl_VertexID==2){
    vCoord = vec2(0.f,1.f);
    gl_Position = mvp*vec4(-1.f,0.f,+1.f,1.f);
  }
  if(gl_VertexID==3){
    vCoord = vec2(0.f,1.f);
    gl_Position = mvp*vec4(-1.f,0.f,+1.f,1.f);
  }
  if(gl_VertexID==4){
    vCoord = vec2(1.f,0.f);
    gl_Position = mvp*vec4(+1.f,0.f,-1.f,1.f);
  }
  if(gl_VertexID==5){
    vCoord = vec2(1.f,1.f);
    gl_Position = mvp*vec4(+1.f,0.f,+1.f,1.f);
  }

 
}
#endif



#ifdef FRAGMENT_SHADER
in highp vec2 vCoord;
out highp vec4 fColor;
void main(){
  highp float dist = distance(vCoord,vec2(0.5f,0.5f));
  if(dist > 0.3f)fColor = vec4(1.f,1.f,1.f,1.f);
  else fColor = vec4(1.f,0.f,0.f,1.f);
}
#endif
