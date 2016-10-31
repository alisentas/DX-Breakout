all:
	g++ `pkg-config --cflags --libs allegro-5 allegro_font-5 allegro_primitives-5 allegro_acodec-5 allegro_image-5 allegro_ttf-5 allegro_audio-5 allegro_main-5 allegro_video-5 allegro_color-5 allegro_memfile-5 allegro_dialog-5 allegro_physfs-5` -o DX main.cpp brick.cpp ball.cpp player.cpp

clear:
	rm DX
