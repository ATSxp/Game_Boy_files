#include "../include/player.h"

Ship player( ( SCREEN_WIDTH - 16 ) / 2,  ( SCREEN_HEIGHT - 16 ) / 2, MAX_HP_PLAYER);
std::vector< Ship > pb; // Player Bullets
u16 MAX_PLAYER_TIMER_SHOOT = 10;
s16 player_timer_shoot = MAX_PLAYER_TIMER_SHOOT, check_slot_bul = 1;
s16 p_boost_bullets = 0, p_points = 0, p_mega_bullets = 1, p_potions = 0;

void initPlayer(){
    loadPalObj(spr_player);
    loadTileObj(spr_player, 16);
    loadTileObj(spr_bullets, 16);
    loadTileObj(spr_bullet_explosion, 16);

    SPRITE_TOTAL_OAM += 33;

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
        lifePlayer();
    }
    
    if( p_mega_bullets > 1 ){
        p_mega_bullets = 1;
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
    }else if( key_hit( KEY_L ) && key_hit( KEY_R ) && p_mega_bullets > 0 ){
        p_mega_bullets--;
        newMegaBulletPlayer();
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

void newMegaBulletPlayer(){
    if( pb.size() < MAX_PLAYER_BULLETS + 1 ){
        if( spriteIsHided( 61 ) ){
            Ship b( player.pos.x, player.pos.y, 2 );
            b.sp.newSprite( 61 );
            b.sp.setAttr( ATTR0_4BPP | ATTR0_SHAPE(0), ATTR1_SIZE_32 );
            b.sp.tid = 94;
            b.size = { 32, 32 };
            
            b.spd = 2;
            b.dy = -b.spd;
            pb.push_back( Ship( b ) );
        }
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

void lifePlayer(){
    if( player.hp > MAX_HP_PLAYER ){
        player.hp = MAX_HP_PLAYER;
    }

    if( key_hit( KEY_B ) && p_potions > 0 && player.hp < MAX_HP_PLAYER ){
        p_potions--;
        player.hp += 2;
    }
}
