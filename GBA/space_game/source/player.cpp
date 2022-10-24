#include "../include/player.h"

Ship player( ( SCREEN_WIDTH - 16 ) / 2,  ( SCREEN_HEIGHT - 16 ) / 2, MAX_HP_PLAYER);
std::vector< Ship > pb; // Player Bullets
s16 player_timer_shoot = MAX_PLAYER_TIMER_SHOOT, check_slot_bul = 1;

void initPlayer(){
    loadPalObj(spr_player);
    loadTileObj(spr_player, 16);
    loadTileObj(spr_bullets, 16);
    loadTileObj(spr_bullet_explosion, 16);

    SPRITE_TOTAL_OAM += 32;

    player.sp.newSprite(0);
    player.sp.setAttr( ATTR0_4BPP | ATTR0_SHAPE(0), ATTR1_SIZE_16);
    player.sp.setTileId(0);
    player.spd = 2;
}

void updatePlayer(){
    if( !player.dead ){
        movePlayer();
        playerShoot();
        animPlayer();
        updateBulletsPlayer();
    }
    gameOverPlayer();
    player.update();
}

void movePlayer(){
    player.dx = 0;
    player.dy = 0;

    if( key_is_down( KEY_UP ) && player.pos.y > 0 ){
        player.dy = -player.spd;
    }else if( key_is_down( KEY_DOWN ) && player.pos.y + player.size.h < SCREEN_HEIGHT ){
        player.dy = player.spd;
    }

    if( key_is_down( KEY_LEFT ) && player.pos.x > 0 ){
        player.dx = -player.spd;
    }else if( key_is_down( KEY_RIGHT ) && player.pos.x + player.size.w < SCREEN_WIDTH ){
        player.dx = player.spd;
    }
}

void playerShoot(){
    if( key_is_down( KEY_A ) && player_timer_shoot <= 0 ){
        newBulletPlayer();
    }
}

void animPlayer(){
    if( player.dx != 0 ){
        if( player.dx < 0 ){ player.sp.flip_h = TRUE; }else { player.sp.flip_h = FALSE; }
        player.sp.anim(2, 2, 8);
    }else {
        player.sp.anim(0, 2, 8);
    }
}

void newBulletPlayer(){
    if( pb.size() < MAX_PLAYER_BULLETS ){
        if( spriteIsHided( check_slot_bul ) ){
            Ship b( player.pos.x, player.pos.y, 1 );
            b.sp.newSprite( check_slot_bul );
            b.sp.setAttr( ATTR0_4BPP | ATTR0_SHAPE(0), ATTR1_SIZE_16 );
            b.sp.setTileId(4);
            
            b.spd = 4;
            b.dy = -b.spd;
            pb.push_back( Ship( b ) );
        }
        check_slot_bul++;
        player_timer_shoot = MAX_PLAYER_TIMER_SHOOT;
    }
}

void updateBulletsPlayer(){
    player_timer_shoot--;
    if( check_slot_bul > MAX_PLAYER_BULLETS - 1 ){ check_slot_bul = 1; }

    size_t i;
    for( i = 0; i < pb.size(); i++ ){
        pb[i].update();

        if( pb[i].pos.y < -32 ){
            destroyPlayerBullet(i);
        }
    }
}

void destroyPlayerBullet( int i ){
    pb[i].dead = TRUE;
    pb[i].sp.hide();
    pb.erase( pb.begin() + i );
}

void gameOverPlayer(){
    for( size_t i = 0; i < enemies.size(); i++ ){
        if( player.shipVsShip( &enemies[i]) && 
            !player.dead && !enemies[i].dead )
        {
            player.setDamage(1);
            enemies[i].dead = TRUE;
        }
    }

    for( size_t j = 0; j < pb.size(); j++ ){
        if( player.dead ){
            pb[j].sp.hide();
            pb.erase( pb.begin() + j );
        }
    }
}
