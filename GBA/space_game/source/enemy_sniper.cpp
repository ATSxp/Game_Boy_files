#include "../include/enemy_sniper.h"

u16 MAX_SNIPER_TIMER_SPAWN = 200, MAX_ORB_TIMER_SPAWN = 50, timer_to_spawn_sniper, timer_to_spawn_orb, check_slot_to_snipers, check_slot_to_orbs;
vector<Ship>snipers, s_orbs;
vu16 _rand_pos, _old_x, _old_y, _p_old_x, _p_old_y;
BOOL s_ready;

void initSniper(){
    loadTileObj(spr_enemy_sniper, 16);
    loadTileObj(spr_projectile_for_sniper, 8);

    timer_to_spawn_sniper = MAX_SNIPER_TIMER_SPAWN;
    timer_to_spawn_orb = MAX_ORB_TIMER_SPAWN;
    
    check_slot_to_snipers = 0;
    check_slot_to_orbs = 0;

    snipers.clear();
    s_orbs.clear();
    SPRITE_TOTAL_OAM += 2;
}

void newSniper(){
    vu32 jump_slots = 62;

    if( timer_to_spawn_sniper <= 0 ){
        if( snipers.size() < MAX_SNIPERS ){
            vs32 nx = qran_range(0, 240 - 16), ny = qran_range(SCREEN_HEIGHT + 16, SCREEN_HEIGHT + 32);
            if( spriteIsHided( jump_slots + check_slot_to_snipers ) ){
                Ship s( nx, ny, 4 );
                s.id = ID_ENEMY_SNIPER;
                s.sp.newSprite( jump_slots + check_slot_to_snipers );
                s.sp.setAttr( ATTR0_4BPP | ATTR0_SHAPE(0), ATTR1_SIZE_16 );

                s.spd = 1;
                snipers.push_back( Ship( s ) );

                s_ready = FALSE;
                _rand_pos = qran_range( 64, 144 );
            }
            check_slot_to_snipers++;
        }
        timer_to_spawn_sniper = MAX_SNIPER_TIMER_SPAWN;
    }
}

void newOrb(){
    vu32 jump_slots = 63;
    if( timer_to_spawn_orb <= 0 ){
        if( s_orbs.size() < MAX_ORBS ){
            if( spriteIsHided( jump_slots + check_slot_to_orbs ) ){
                _p_old_x = player.pos.x + player.size.w / 2;
                _p_old_y = player.pos.y + player.size.h / 2;

                Ship o( _old_x + 16 / 2, _old_y + 16 / 2, 1 );
                o.id = ID_SNIPER_ORB;
                o.sp.newSprite( jump_slots + check_slot_to_orbs );
                o.sp.setAttr( ATTR0_4BPP | ATTR0_SHAPE(0), ATTR1_SIZE_8 );
                o.sp.tid = 156 + ( 4 * 3 );
                o.size = { 8, 8 };
                o.spd = ( 1 >> 14 ) >> 14;

                s16 ddx = _p_old_x - o.pos.x, ddy = _p_old_y - o.pos.y;
                s16 a = ArcTan2(ddx, ddy);

                o.dx = ( ( lu_cos(a) >> 8 ) - ( o.spd << 4 ) ) >> 2;
                o.dy = ( ( lu_sin(a) >> 8 ) - ( o.spd << 4 ) ) >> 2;

                s_orbs.push_back( Ship( o ) );
            }
            check_slot_to_orbs++;
        }
        timer_to_spawn_orb = MAX_ORB_TIMER_SPAWN;
    }
}

void updateSniper(){
    newSniper();
    newOrb();

    timer_to_spawn_sniper--;
    if( s_ready ) timer_to_spawn_orb--;
    if( check_slot_to_snipers > MAX_SNIPERS - 1 ){ check_slot_to_snipers = 0; }
    if( check_slot_to_orbs    > MAX_ORBS    - 1 ){ check_slot_to_orbs    = 0; }

    for( size_t i = 0; i < snipers.size(); i++ ){
        removeEnemies( &snipers[i] );

        _old_x = snipers[i].pos.x;
        _old_y = snipers[i].pos.y;

        if( snipers[i].dead ){
            destroyEnemy( &snipers[i], &snipers, i );
            s_ready = FALSE;
        }else {
            if( snipers[i].pos.y < player.pos.y ){
                s_ready = TRUE;
                snipers[i].sp.anim(39, 2);
            }else {
                snipers[i].sp.tid = 156 + ( 4 * 2 );
            }

            if( snipers[i].pos.y < SCREEN_HEIGHT - _rand_pos ){
                snipers[i].dy = 0;
            }else {
                snipers[i].dy = -snipers[i].spd;
            }
        }

        snipers[i].update();
    }

    for( size_t j = 0; j < s_orbs.size(); j++ ){
        s_orbs[j].sp.anim( 168, 2 );

        checkPlayerDamage(&s_orbs[j]);

        if( s_orbs[j].dead || 
            ( s_orbs[j].pos.x < - s_orbs[j].size.w || s_orbs[j].pos.x > SCREEN_WIDTH || 
                s_orbs[j].pos.y < - s_orbs[j].size.h || s_orbs[j].pos.y > SCREEN_HEIGHT )
        ){
            s_orbs[j].sp.hide();
            s_orbs.erase( s_orbs.begin() + j );
        }
        
        s_orbs[j].update();
    }
}
