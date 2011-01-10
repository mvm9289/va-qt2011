
README: Shaders

INSTRUCCIONS:

 - Primerament, cal carregar els codis per al vertex shader (VShader.vert)
	i per al fragment shader (FShader.frag) (Si es requereix, carregar també el
	geometry shader (GShader.geom).

 - Seguidment, cal carregar manualment les textures a la pestanya de "Textures"
	del Shader Maker:
		- Una serà per al mapa de normals.
		- Una altra serà el fons del mar.
		- I finalment, la última serà la textura que representa l'aigua.

 - Finalment, a la pestanya de "Uniforms" es pot variar la densitat del aigua amb
	la qual cosa veurem més o menys el fons del mar segons la nostra posició.

--------------------------------------------------------------------------------------

FUNCIONALITATS IMPLEMENTADES:

 - Modificació dels vertex amb les funcions sinusoidals.
 - Moviment de la textura de l' aigua i el normal map
 - Normal mapping
 - Iluminació Blinn-Phong
 - Aproximació de les equacions de Fresnel

--------------------------------------------------------------------------------------

VIDEOS demostratius del shader en alguns moments del desemvolupament:

 - http://www.youtube.com/watch?v=bYIyiJ0SzhU
 - http://www.youtube.com/watch?v=8v-c1No1cfc
 - http://www.youtube.com/watch?v=Ula0zs72Kr8 (Versió entregada, tot i que no
	s' aprecia massa l' aplicació de les equacions de Fresnel).