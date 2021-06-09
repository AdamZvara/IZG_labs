#ifdef VERTEX_SHADER

uniform mat4 view;
uniform mat4 proj;
#line 5

void main(){
  if(gl_VertexID>2){
    gl_Position = vec4(0.f,0.f,0.f,1.f);
    return;
  }
  gl_Position=vec4(gl_VertexID%2,gl_VertexID/2,0.f,1.f);
}
#endif



#ifdef FRAGMENT_SHADER
out highp vec4 fColor;
void main(){
  fColor = vec4(1.f);
}
#endif
