#include "../include/game.h"

extern u32 world_seconds;

using namespace std;

int spc0_y, ptx;
BOOL ready = FALSE;
// Imortality item
Sprite im;
Sprite imf;

void initGame(){
    sqran(world_seconds);
    REG_DISPCNT = DCNT_MODE0 | DCNT_BG0 | DCNT_BG1 | DCNT_BG2 | DCNT_OBJ_1D | DCNT_OBJ;

    loadPalBg(tileset_space);
    loadTile(tileset_space);

    Map spc0(0, BG_CBB(0) | BG_SBB(28) | BG_4BPP | BG_PRIO(2), space_bg0, 32, 64);
    Map spc1(1, BG_CBB(0) | BG_SBB(25) | BG_4BPP | BG_PRIO(1), space_bg1, 32, 64);

    memcpy16(pal_bg_bank[15], tileset_space_pal_bin, tileset_space_pal_bin_size / 2);

    im.newSprite(60);
    imf.newSprite(55);

    spc0_y = 0;

    tte_init_chr4c(
            2, 
            BG_CBB(1) | BG_SBB(31) | BG_PRIO(0), 
            0xF000, 
            0x0201,
            0x28A3 | 0x288F << 16, 
            &verdana9Font, 
            (fnDrawg)chr4c_drawg_b1cts_fast);

    initVoid();
    initPlayer();
    initEnemy();
    initConvoys();
    initHudPlayer();
    initGameOver();
    initSniper();
}

void updateGame(){
    if( !ready ){
        im.hide();
        imf.hide();

        player.dy = -( player.spd >> 1 );

        if( player.pos.y < ( SCREEN_HEIGHT >> 1 ) + 16 ){
            player.dy = 0;
            p_can_move = TRUE;
            ready = TRUE;
        }
    }else {
        updateEnemies();
        updateConvoys();
        updateSniper();

        if( player.dead ){
            updateGameOver();
        }else {
            updateHudPlayer();
        }

    }

    updatePlayer();

    spc0_y--;

    REG_BG_OFS[0].y = spc0_y;
    REG_BG_OFS[1].y = spc0_y * 3;

    updateShakeScreen();

    updateVoid();
}

void endGame(){
    /* tte_erase_screen(); */
    resetVoid();
    RegisterRamReset(RESET_PALETTE);
    RegisterRamReset(RESET_VRAM);
    RegisterRamReset(RESET_OAM);
}

Scene game_scene = {
    initGame,
    endGame,
    updateGame,
};

void initHudPlayer(){
    loadTileObj(spr_hp_player, 8);
    loadTileObj(spr_mega_bullet_slot_and_item , 16);
    loadTileObj(spr_life_item , 16);
    loadTileObj(spr_boost_bullet_item , 16);
    loadTileObj(spr_multi_bullets_item, 16);
    loadTileObj(spr_imortality_item, 16);
    loadTileObj(spr_buttons, 16);
    loadTileObj(spr_mega_bullet, 32);
    loadTileObj(spr_p_imortality_effect, 16);

    SPRITE_TOTAL_OAM += 5;

    im.setAttr(ATTR0_4BPP | ATTR0_SHAPE(0), ATTR1_SIZE_16x16);
    imf.setAttr(ATTR0_4BPP | ATTR0_SHAPE(0), ATTR1_SIZE_16x16);
}

void updateHudPlayer(){
    // Points
    string points_txt = translTxt(POINTS);
    ptx = SCREEN_WIDTH - ( ( points_txt.length() - 7 ) * 5 );

    tte_erase_screen();
    tte_set_pos(ptx, 0);
    tte_write_str("#{ci:7}"+points_txt);

    // Hp
    for( int i = 0; i < MAX_HP_PLAYER; i++ ){
        OBJ_ATTR *spr = &OBJ_BUFFER[ ( 16 + MAX_HP_PLAYER ) + i ];
        obj_set_attr( spr, 
                ATTR0_4BPP | ATTR0_SHAPE(0), 
                ATTR1_SIZE_8, 
                ATTR2_BUILD(73, 0, 0) );
        obj_set_pos( spr, i * 9 + 2, SCREEN_HEIGHT - 12 );
    }

    for( int i = 0; i < player.hp; i++ ){
        OBJ_ATTR *spr = &OBJ_BUFFER[ 16 + i ];
        obj_set_attr( spr, 
                ATTR0_4BPP | ATTR0_SHAPE(0), 
                ATTR1_SIZE_8, 
                ATTR2_BUILD(72, 0, 0) );
        obj_set_pos( spr, -( i - 1 ) * 9 + ( 8 * 7 ), SCREEN_HEIGHT - 12 );

    }

    obj_hide_multi( &OBJ_BUFFER[ 16 ], ABS( player.hp - MAX_HP_PLAYER ) );

    // Mega Bullet
    OBJ_ATTR *spr = &OBJ_BUFFER[56];
    obj_set_attr(spr, ATTR0_4BPP | ATTR0_SHAPE(0), ATTR1_SIZE_16, ATTR2_BUILD(74, 0, 0) );
    obj_set_pos(spr, SCREEN_WIDTH - 16, SCREEN_HEIGHT - 16 );

    if( p_mega_bullets > 0 ){
        obj_unhide(spr, 0);
    }else {
        obj_hide(spr);
    }

    // Potion
    OBJ_ATTR *po = &OBJ_BUFFER[57];
    obj_set_attr(po, ATTR0_4BPP | ATTR0_SHAPE(0), ATTR1_SIZE_16, ATTR2_BUILD(78, 0, 0) );
    obj_set_pos(po, 0, SCREEN_HEIGHT - 32 );

    if( p_potions > 0 ){
        obj_unhide(po, 0);
        tte_write_str("#{P:16,130}" + to_string(p_potions) );
    }else {
        obj_hide(po);
        chr4c_erase(16, 136, 32, 8);
    }

    //Multi Bullet
    OBJ_ATTR *m = &OBJ_BUFFER[58];
    obj_set_attr(m, ATTR0_4BPP | ATTR0_SHAPE(0), ATTR1_SIZE_16, ATTR2_BUILD(86, 0, 0) );
    obj_set_pos(m, SCREEN_WIDTH - 33, SCREEN_HEIGHT - 16 );

    if( p_multi_bullets > 0 ){
        obj_unhide(m, 0);
    }else {
        obj_hide(m);
    }

    //Boost Bullet
    OBJ_ATTR *b = &OBJ_BUFFER[59];
    obj_set_attr(b, ATTR0_4BPP | ATTR0_SHAPE(0), ATTR1_SIZE_16, ATTR2_BUILD(82, 0, 0) );
    obj_set_pos(b, SCREEN_WIDTH - 49, SCREEN_HEIGHT - 16 );

    if( p_boost_bullets > 0 ){
        obj_unhide(b, 0);
    }else {
        obj_hide(b);
    }

    // Imortality
    if( p_imortal_item > 0 ){
        im.tid = 90;
        im.prio = 0;

        im.setPos( SCREEN_WIDTH - 65, SCREEN_HEIGHT - 16 );
        im.unhide();
    }else {
        im.hide();
    }
    im.update();

    if( p_imortal ){
        imf.anim(31, 7, 8);
        imf.setPos( player.pos.x, player.pos.y );
        imf.update();
        imf.unhide();
    }else{ imf.hide(); }
}

void removeEnemies( Ship *t ){
    for( size_t j = 0; j < pb.size(); j++ ){
        if( t->shipVsShip( &pb[j] ) && !t->damaged && !t->dead && !pb[j].dead && pb[j].pos.y > 0 ){
            t->setDamage(1);

            if( pb[j].hp < 2 ){
                destroyPlayerBullet( j );
            }else {
                if( t->hp > 0 ){
                    pb[j].dy = 0;
                }else {
                    pb[j].dy = -pb[j].spd;
                }
            }
        }
    }

    if( t->damaged ){
        t->sp.pal = 1;
    }else {
        t->sp.pal = 0;
    }
}

void animEnemyExplode( Ship *t ){
    if( t->dead ){
        if( t->sp.pal > 0 ) t->sp.pal = 0;
        if( t->dy != 0 ) t->dy = 0;
        t->sp.anim(5, 9, 2);
    }
}

void destroyEnemy( Ship *t, std::vector<Ship> *v, size_t i ){
    t->dead = TRUE;
    animEnemyExplode( t );
    if( t->sp.tid == ( 4 + 9 ) * 4 && t->dead ){
        if( t->hp <= 0 ){ 
            switch( t->id ){
                case ID_ENEMY_SNIPER:
                    p_points += 2;
                    break;
                default:
                    p_points++;
                    break;
            }
        }

        t->sp.hide();
        v->erase( v->begin() + i );
    }
}

void destroyPlayerBullet( int i ){
    pb[i].dead = TRUE;
    pb[i].sp.hide();
    pb.erase( pb.begin() + i );
}

void checkPlayerDamage( Ship *t ){
    if( player.shipVsShip( t ) && 
        !player.dead && !t->dead && t->pos.y < SCREEN_HEIGHT - 16 )
    {
        if( !p_imortal ){player.setDamage(1); shakeScreen( 20, 1, 3); }
        t->dead = TRUE;
    }
}
