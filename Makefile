all:
	g++ -o DX *.cpp $$(pkg-config --libs allegro-5 allegro_primitives-5 allegro_acodec-5 allegro_image-5 allegro_ttf-5)

clear:
	rm DX
