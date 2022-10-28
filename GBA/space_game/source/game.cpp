#include "../include/game.h"

using namespace std;

Map spc0(0, BG_CBB(0) | BG_SBB(28) | BG_4BPP | BG_PRIO(2), space_bg0, 32, 64);
Map spc1(1, BG_CBB(0) | BG_SBB(25) | BG_4BPP | BG_PRIO(1), space_bg1, 32, 64);
int spc0_y = 0, ptx;

void initGame(){
    REG_DISPCNT = DCNT_MODE0 | DCNT_BG0 | DCNT_BG1 | DCNT_BG2 | DCNT_OBJ_1D | DCNT_OBJ;

    loadPalBg(tileset_space);
    loadTile(tileset_space);

    tte_init_se(
            2, 
            BG_CBB(1) | BG_SBB(31) | BG_PRIO(0), 
            0, 
            0x77DF, 
            14, 
            NULL, 
            NULL);

    initVoid();
    initPlayer();
    initEnemy();
    initConvoys();
    initHudPlayer();

    loadTileObj(spr_mega_bullet, 32);
}

void updateGame(){
    string points_txt = "Points: " + to_string( p_points );
    ptx = SCREEN_WIDTH - ( points_txt.length() * 8 );

    tte_write_str("#{es;P:" + to_string(ptx) + ",0}");
    tte_write_str(points_txt);

    updateHudPlayer();
    updatePlayer();
    updateEnemies();
    updateConvoys();

    spc0_y--;
    REG_BG_OFS[0].y = spc0_y;
    REG_BG_OFS[1].y = spc0_y * 3;

    updateVoid();
}

void initHudPlayer(){
    loadTileObj(spr_hp_player, 8);
    loadTileObj(spr_mega_bullet_slot_and_item , 16);
    loadTileObj(spr_life_item , 16);
    loadTileObj(spr_boost_bullet_item , 16);
    loadTileObj(spr_multi_bullets_item, 16);
    loadTileObj(spr_imortality_item, 16);

    SPRITE_TOTAL_OAM += 5;
}

void updateHudPlayer(){
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
        tte_write_str("#{P:16,136}" + to_string(p_potions) );
    }else {
        obj_hide(po);
        tte_erase_rect(16, 136, 32, 8);
    }

    //Boost Bullet
    OBJ_ATTR *b = &OBJ_BUFFER[58];
    obj_set_attr(b, ATTR0_4BPP | ATTR0_SHAPE(0), ATTR1_SIZE_16, ATTR2_BUILD(82, 0, 0) );
    obj_set_pos(b, SCREEN_WIDTH - 16, SCREEN_HEIGHT - 32 );

    string boost_text = to_string(p_boost_bullets) + ".lv";
    vu32 tx = SCREEN_WIDTH - ( boost_text.length() * 8 ) - 16;

    if( p_boost_bullets > 0 ){
        obj_unhide(b, 0);
        tte_write_str("#{P:"+ to_string(tx) + ",136}" + boost_text );
    }else {
        obj_hide(b);
        tte_erase_rect(224, 128, 8, 8);
    }
}

void removeEnemies( Ship *t ){
    for( size_t j = 0; j < pb.size(); j++ ){
        if( t->shipVsShip( &pb[j] ) && !t->damaged && !t->dead && !pb[j].dead ){
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
}

void animEnemyExplode( Ship *t ){
    if( t->dead ){
        t->sp.pal = 0;
        t->dy = 0;
        t->sp.anim(5, 9, 2);
    }
}

void destroyEnemy( Ship *t, std::vector<Ship> *v, size_t i ){
    t->dead = TRUE;
    animEnemyExplode( t );
    if( t->sp.tid == ( 4 + 9 ) * 4 && t->dead ){
        if( t->hp <= 0 ){
            p_points++;
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
