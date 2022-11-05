#include "../include/player.h"

Ship player( ( SCREEN_WIDTH - 16 ) / 2, SCREEN_HEIGHT, MAX_HP_PLAYER);
std::vector< Ship > pb; // Player Bullets
u16 MAX_PLAYER_TIMER_SHOOT = 12, p_imortality_timer;
s16 player_timer_shoot, check_slot_bul, p_boost_bullets = 1, p_points = 0, p_mega_bullets = 1, p_potions = 1, p_multi_bullets = 1, p_imortal_item = 1;
BOOL p_imortal, p_can_move;

void initPlayer(){
    loadPalObj(spr_player);
    loadTileObj(spr_player, 16);
    loadTileObj(spr_bullets, 16);
    loadTileObj(spr_bullet_explosion, 16);

    SPRITE_TOTAL_OAM += 33;

    player.id = ID_PLAYER;
    player.sp.newSprite(0);
    player.sp.setAttr( ATTR0_4BPP | ATTR0_SHAPE(0), ATTR1_SIZE_16);
    player.sp.setTileId(0);
    player.spd = 2;

    p_imortality_timer = MAX_PLAYER_IMORTALITY;
    check_slot_bul = 1;
    player_timer_shoot = MAX_PLAYER_TIMER_SHOOT;
    p_imortal = FALSE;
    p_can_move = FALSE;

    pb.clear();
}

void updatePlayer(){
    if( !player.dead ){
        movePlayer();
        playerShoot();
        animPlayer();
        updateBulletsPlayer();
        lifePlayer();
        useImortalityItem();
    }else {
        deadPlayer();
    }
    
    if( p_mega_bullets > 1 ){
        p_mega_bullets = 1;
    }else if( p_imortal_item > 1 ){
        p_imortal_item = 1;
    }

    gameOverPlayer();
    player.update();
}

void movePlayer(){
    if( p_can_move ){
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
}

void playerShoot(){
    if( p_can_move ){
        if( key_is_down( KEY_A ) && player_timer_shoot <= 0 ){
            newBulletPlayer();
        }else if( key_hit( KEY_L ) && p_mega_bullets > 0 ){
            p_mega_bullets--;

            shakeScreen(100, 2, 30);
            shakeScreen(100, 1, 20);
            shakeScreen(100, 0, 10);

            newMegaBulletPlayer();
        }
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
            b.id = ID_PLAYER_BULLET;
            b.sp.newSprite( check_slot_bul );
            b.sp.setAttr( ATTR0_4BPP | ATTR0_SHAPE(0), ATTR1_SIZE_16 );
            b.sp.setTileId(4);
            
            b.spd = 4 + p_boost_bullets;
            b.dy = -b.spd;
            pb.push_back( Ship( b ) );
        }
        player_timer_shoot = MAX_PLAYER_TIMER_SHOOT;
        check_slot_bul++;
    }
}

void newMegaBulletPlayer(){
    if( pb.size() < MAX_PLAYER_BULLETS + 1 ){
        if( spriteIsHided( 61 ) ){
            Ship b( player.pos.x - player.size.w / 2, player.pos.y, 2 );
            b.sp.newSprite( 61 );
            b.sp.setAttr( ATTR0_4BPP | ATTR0_SHAPE(0), ATTR1_SIZE_32 );
            b.sp.tid = 106;
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

    for( size_t i = 0; i < pb.size(); i++ ){
        if( pb[i].pos.y < -64 ){
            pb[i].dy = 0;
            destroyPlayerBullet(i);
        }

        pb[i].update();
    }
}

void gameOverPlayer(){
    for( size_t i = 0; i < enemies.size(); i++ ) checkPlayerDamage(&enemies[i]);
    for( size_t k = 0; k < snipers.size(); k++ ) checkPlayerDamage(&snipers[k]);

    for( size_t j = 0; j < pb.size(); j++ ){
        if( player.dead ){
            pb[j].sp.hide();
            pb.clear();
        }
    }
}

void lifePlayer(){
    if( player.hp > MAX_HP_PLAYER ){
        player.hp = MAX_HP_PLAYER;
    }

    if( key_hit( KEY_B ) && p_potions > 0 && player.hp < MAX_HP_PLAYER && p_can_move ){
        p_potions--;
        player.hp += qran_range(1, MAX_HP_PLAYER - 1 );
    }
}

void useImortalityItem(){
    if( p_imortal ){p_imortality_timer--;}
    if( p_imortality_timer <= 0 ){ p_imortality_timer = MAX_PLAYER_IMORTALITY; p_imortal = FALSE; }

    if( key_hit( KEY_R ) && p_imortal_item > 0 && !p_imortal && p_can_move ){
        p_imortal_item--;
        p_imortal = TRUE;
    }
}

void deadPlayer(){
    player.sp.anim(5, 9, 2);
    if( player.sp.tid == ( 4 + 9 ) * 4 && player.dead ){
        player.sp.hide();
    }
}
