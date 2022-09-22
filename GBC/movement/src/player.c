// Player
struct entity{
    uint8_t sp;
    uint8_t x;
    uint8_t y;
    uint8_t w;
    uint8_t h;
    uint8_t dx;
    uint8_t dy;
    uint8_t speed;
}entity;

struct entity player;

void playerLoad(){
    player.x = 0;
    player.y = 0;
    player.w, player.h = 8;
    player.speed = 8;
    player.dx, player.dy = 0;

    set_sprite_data(0, 4, player_sprites);
}

void playerUpdate(){

    if(joypad() == J_UP && player.y > 0){
        player.dy = - player.speed;
        player.sp = 3;
    }else if(joypad() == J_DOWN && player.y < (8 * 12)){
        player.dy = player.speed;
        player.sp = 0;
    }else{
        player.dy = 0;
    }

    if(joypad() == J_LEFT && player.x > 0){
        player.dx = - player.speed;
        player.sp = 1;
    }else if(joypad() == J_RIGHT && player.x < (8 * 19)){
        player.dx = player.speed;
        player.sp = 2;
    }else{
        player.dx = 0;
    }

    move_sprite(0, player.x + 8, player.y + 16);

    player.x += player.dx;
    player.y += player.dy;
}

void playerDraw(){
    set_sprite_tile(0, player.sp);
}