
uniform float time;


varying vec3 position;

void main( void )
{
	float scale = 3.3;
	float x = gl_Vertex.x;
	float y = gl_Vertex.y;

	float s = sin( ( time + 10.0 * y ) * scale );
	float c = cos( ( time + 10.0 * x ) * scale );
	float z = 0.02 * s * c;

	vec3 v = gl_Vertex.xyz + gl_Normal * z;

	position = v;
	gl_Position = gl_ModelViewProjectionMatrix * vec4( v, 1.0 );
	gl_TexCoord[0] = gl_MultiTexCoord0;
}

