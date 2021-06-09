#ifdef VERTEX_SHADER

uniform mat4 view;
uniform mat4 proj;
#line 5

void main(){
  uint indices[] = uint[](
    0u,1u,2u,2u,1u,3u,
    4u,5u,6u,6u,5u,7u,
    0u,4u,2u,2u,4u,6u,
    1u,5u,3u,3u,5u,7u,
    0u,1u,4u,4u,1u,5u,
    2u,3u,6u,6u,3u,7u
  );
  if(gl_VertexID>=indices.length()){
    gl_Position = vec4(0.f,0.f,0.f,1.f);
    return;
  }

  vec3 pos;
  for(uint i=0u;i<3u;++i)
    pos[i] = float((indices[gl_VertexID]>>i)&1u);

  gl_Position = proj*view*vec4(pos*2.f-1.f,1.f);
}
#endif



#ifdef FRAGMENT_SHADER
out highp vec4 fColor;
void main(){
  fColor = vec4(1.f);
}
#endif
