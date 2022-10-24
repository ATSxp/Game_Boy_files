#include "../include/enemy_common.h"

std::vector< Ship > enemies; // Todos os inimigos
int timer_to_spawn_enemy = MAX_ENEMY_TIMER_SPAWN, check_slot_to_enemy = 1;

void initEnemy(){
    loadTileObj(spr_enemy_common, 16);
    enemies.clear();
    SPRITE_TOTAL_OAM += 16;
}

void newEnemy(){
    vu32 jump_slots = 31;

    if( timer_to_spawn_enemy <= 0 ){
        vs32 nx = qran_range(0, 240 - 16), ny = qran_range(-16, -64);
        if( enemies.size() < MAX_ENEMIES ){
            if( spriteIsHided( jump_slots + check_slot_to_enemy ) ){
                Ship e( nx, ny, 5 );
                e.sp.newSprite( jump_slots + check_slot_to_enemy );
                e.sp.setAttr( ATTR0_4BPP | ATTR0_SHAPE(0), ATTR1_SIZE_16 );
                e.sp.setTileId(14);
                
                e.spd = 1;
                e.dy = e.spd;
                enemies.push_back( Ship( e ) );
            }
            check_slot_to_enemy++;
            timer_to_spawn_enemy = MAX_ENEMY_TIMER_SPAWN;
        }
    }
}

void updateEnemies(){
    newEnemy();

    timer_to_spawn_enemy--;

    if( check_slot_to_enemy > MAX_ENEMIES ){ check_slot_to_enemy = 1; }

    size_t i;
    for( i = 0; i < enemies.size(); i++ ){
        removeEnemiesAfterPassLimit(i);
        removeEnemies(i);
        if( enemies[i].dead ){
            destroyEnemy(i);
        }else {
            enemies[i].sp.anim(14, 2);
        }

        if( enemies[i].damaged ){
            enemies[i].sp.pal = 1;
        }else {
            enemies[i].sp.pal = 0;
        }

        enemies[i].update();
    }
}

void removeEnemiesAfterPassLimit( size_t i ){
    if( enemies[i].pos.y > SCREEN_HEIGHT ){
        destroyEnemy(i);
    }
}

void removeEnemies( size_t i ){
    for( size_t j = 0; j < pb.size(); j++ ){
        if( enemies[i].shipVsShip( &pb[j] ) && !enemies[i].damaged && !enemies[i].dead ){
            enemies[i].setDamage(1);
        
            pb[j].dead = TRUE;
            pb[j].sp.hide();
            pb.erase( pb.begin() + j );
        }
    }
}

void animEnemyExplode(int i){
    if( enemies[i].dead ){
        enemies[i].sp.pal = 0;
        enemies[i].dy = 0;
        enemies[i].sp.anim(5, 9, 2);
    }
}

void destroyEnemy(int i){
    enemies[i].dead = TRUE;
    animEnemyExplode(i);
    if( enemies[i].sp.tid == ( 4 + 9 ) * 4 && enemies[i].dead ){
        enemies[i].sp.hide();
        enemies.erase( enemies.begin() + i );
    }
}
