#include "../include/enemy_convoy.h"

std::vector< Ship > convoys;
std::vector< Item > items;
u16 timer_to_spawn_convoy, check_slot_to_convoy, check_slot_to_item, timer_to_move, MAX_TIMER_TO_MOVE = 80;

Item_list items_store[5] = {
    { 74, ITEM_MEGA_BULLET },
    { 78, ITEM_POTION },
    { 86, ITEM_MULTI_BULLET },
    { 82, ITEM_BOOST_BULLET },
    { 90, ITEM_IMORTAL }
};

void initConvoys(){
    loadTileObj(spr_enemy_convoy, 16);

    SPRITE_TOTAL_OAM += 8;

    timer_to_spawn_convoy = MAX_CONVOYS_TIMER_SPAWN;
    check_slot_to_convoy = 0;
    convoys.clear();
    items.clear();
}

void newConvoy(){
    vu16 jump_slot = 48;
    vs32 rnd_x = qran_range( 0, 240 - 16 ), rnd_y = qran_range( -16, -32 );

    if( timer_to_spawn_convoy <= 0 ){
        if( convoys.size() < MAX_CONVOYS ){
            if( spriteIsHided( jump_slot + check_slot_to_convoy ) ){
                Ship c( rnd_x, rnd_y, 3 );
                c.id = ID_ENEMY_CONVOY;
                c.sp.newSprite( jump_slot + check_slot_to_convoy );
                c.sp.setAttr( ATTR0_4BPP | ATTR0_SHAPE(0), ATTR1_SIZE_16 );
                c.sp.setTileId(16);

                c.spd = 2;
                c.dy = c.spd;

                convoys.push_back( Ship( c ) );
                timer_to_move = MAX_TIMER_TO_MOVE;
            }
            check_slot_to_convoy++;
            timer_to_spawn_convoy = MAX_CONVOYS_TIMER_SPAWN;
        }
    }
}

void updateConvoys(){
    newConvoy();

    timer_to_spawn_convoy--;
    if( check_slot_to_convoy > MAX_CONVOYS - 1 ){ check_slot_to_convoy = 0; }
    if( check_slot_to_item > MAX_ITEMS - 1 ){ check_slot_to_item = 0; }

    for( size_t i = 0; i < convoys.size(); i++ ){
        removeEnemies( &convoys[i] );

        if( convoys[i].pos.y > SCREEN_HEIGHT ){
            convoys[i].sp.hide();
            convoys[i].dead = TRUE;
            convoys.erase( convoys.begin() + i );
        }else if( convoys[i].hp <= 0 ){
            if( !convoys[i].dead ){
                vu32 _old_x = convoys[i].pos.x, _old_y = convoys[i].pos.y;
                vu32 _rand_item = qran_range(ITEM_MEGA_BULLET, ITEM_IMORTAL);

                if( items.size() < MAX_ITEMS ){
                    if( spriteIsHided( 53 + check_slot_to_item ) ){
                        Item i( 53 + check_slot_to_item, _old_x, _old_y );
                        i.sp.setAttr( ATTR0_4BPP | ATTR0_SHAPE(0), ATTR1_SIZE_16 );
                        i.sp.prio = 1;
                        i.sp.tid = items_store[ _rand_item - 1 ].tid;
                        i.id = items_store[ _rand_item - 1 ].id;
                        i.spd = ( 1 >> 6 ) >> 2;

                        items.push_back( Item( i ) );
                    }
                    check_slot_to_item++;
                }

            }
            destroyEnemy( &convoys[i], &convoys, i );
        }else {
            convoys[i].sp.anim(16, 2);
        }

        if( convoys[i].pos.y >= ( SCREEN_HEIGHT / 2 ) - 32 && timer_to_move > 0 ){
            timer_to_move--;
            convoys[i].dy = 0;
        }else if( timer_to_move <= 0 ){
            convoys[i].dy = convoys[i].spd;
        }

        convoys[i].update();
    }
    updateItems();
}

void updateItems(){
    for( size_t i = 0; i < items.size(); i++ ){
        if( items[i].itemVsShip( &player ) ){
            itemIdValue( items[i].id );

            items[i].sp.hide();
            items[i].dead = TRUE;
            items.erase( items.begin() + i );
        }

        items[i].update();
        items[i].chase( player.pos.x, player.pos.y );
    }
}

void itemIdValue( u16 id ){
    switch(id){
        case ITEM_MEGA_BULLET:
            p_mega_bullets++;
            break;
        case ITEM_POTION:
            p_potions++;
            break;
        case ITEM_MULTI_BULLET:
            p_multi_bullets++;
            MAX_PLAYER_TIMER_SHOOT--;
            break;
        case ITEM_BOOST_BULLET:
            p_boost_bullets++;
            break;
        case ITEM_IMORTAL:
            p_imortal_item++;
            break;
    }
}
