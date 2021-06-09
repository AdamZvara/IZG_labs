#ifdef VERTEX_SHADER

uniform mat4 view;
uniform mat4 proj;
#line 5

out highp vec2 vCoord;
void main(){

  //fullscreen quad
  highp vec2 verts[]= vec2[](
    vec2(-1.f,-1.f),
    vec2(+1.f,-1.f),
    vec2(-1.f,+1.f),
    vec2(-1.f,+1.f),
    vec2(+1.f,-1.f),
    vec2(+1.f,+1.f)
  );

  if(gl_VertexID>=verts.length()){
    gl_Position = vec4(0.f,0.f,0.f,1.f);
    return;
  }

  gl_Position = vec4(verts[gl_VertexID],0.f,1.f);
  vCoord = verts[gl_VertexID]*.5f+.5f;
}
#endif



#ifdef FRAGMENT_SHADER
in highp vec2 vCoord;
out highp vec4 fColor;
void main(){
  fColor = vec4(vCoord,0.f,1.f);
}
#endif
