
uniform sampler2D waterTex;
uniform sampler2D seabedTex;
uniform sampler2D normalMap;
uniform float time;
uniform float waterDensity;

varying vec3 position;

vec4 computeBlinnPhong( vec3 normal, vec4 color )
{
	gl_LightSourceParameters light = gl_LightSource[0];

	vec3 obs = ( gl_ModelViewMatrixInverse * vec4( 0.0, 0.0, 0.0, 1.0 ) ).xyz;
	vec4 lightPos = gl_ModelViewMatrixInverse * light.position;

	float dist = length( lightPos.xyz - position );
	dist = dist * dist;
	vec3 L = normalize( lightPos.xyz - position );
	vec3 H = normalize( L + ( obs - position ) );

	float NdotL = max( 0.0, dot( normal, L ) );
	float NdotH = max( 0.0, dot( normal, H ) );

	float Idiff = NdotL;
	float Ispec = pow( NdotH, gl_FrontMaterial.shininess );

	return
		color * gl_LightModel.ambient +
		color * light.ambient +
		color * (light.diffuse  * Idiff) / dist +
		color * (light.specular * Ispec) / dist;
}

vec4 computeFresnel( vec4 waterColor, vec4 seabedColor )
{
	vec3 obs = ( gl_ModelViewMatrixInverse * vec4( 0.0, 0.0, 0.0, 1.0 ) ).xyz;

	float waterIOR = 1.333;
	float airIOR = 1.0003;
	float Rs = ( airIOR - waterIOR ) / ( airIOR + waterIOR );

	float Tv = normalize( obs - position ).z;

	float F = Rs + ( 1.0 - Rs ) * ( 1.0 - Tv );
	if (F > 1.0) F = 1.0;
	else if (F < 0.0) F = 0.0;
	
	return (F * waterColor) + ((1.0 - F) * seabedColor);
}

vec4 computeFragmentColor( vec3 normal, vec4 waterColor, vec4 seabedColor )
{
	vec4 fresnelColor = computeFresnel( waterColor, seabedColor );

	return computeBlinnPhong( normal, fresnelColor );
}

vec3 computeNormal()
{
	vec2 texCoord = 2.0 * vec2(  gl_TexCoord[0].s + time*0.06,
								 gl_TexCoord[0].t - time*0.06  );

	return normalize( 2.0 * vec3( texture2D( normalMap, texCoord ) ) + 1.0 );
}

vec4 computeSeabedColor( vec4 waterColor )
{
	vec2 texCoord = 3.0 * gl_TexCoord[0].st;

	return (( 1.0 - waterDensity ) * texture2D( seabedTex, texCoord )) + ( waterDensity * waterColor );
}

vec4 computeWaterColor()
{
	vec2 texCoord = vec2(  gl_TexCoord[0].s + time*0.06,
						   gl_TexCoord[0].t - time*0.06  );

	return texture2D( waterTex, texCoord );
}

void main( void )
{
	vec4 waterColor = computeWaterColor();
	vec4 seabedColor = computeSeabedColor( waterColor );
	vec3 normal = computeNormal();

	gl_FragColor = computeFragmentColor( normal, waterColor, seabedColor );
}
