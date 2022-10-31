#include "../include/enemy_convoy.h"

std::vector< Ship > convoys;
std::vector< Item > items;
u16 timer_to_spawn_convoy, check_slot_to_convoy, check_slot_to_item;
int item_speed = 1;
u8 drop_item;

void initConvoys(){
    loadTileObj(spr_enemy_convoy, 16);

    SPRITE_TOTAL_OAM += 8;

    timer_to_spawn_convoy = MAX_CONVOYS_TIMER_SPAWN;
    check_slot_to_convoy = 0;
    convoys.clear();
    items.clear();
    drop_item = FALSE;
}

void newConvoy(){
    vu16 jump_slot = 48;

    if( timer_to_spawn_convoy <= 0 ){
        vs32 rnd_x = qran_range( 0, 240 - 16 ), rnd_y = qran_range( -16, -32 );
        if( convoys.size() < MAX_CONVOYS ){
            if( spriteIsHided( jump_slot + check_slot_to_convoy ) ){
                Ship c( rnd_x, rnd_y, 3 );
                c.sp.newSprite( jump_slot + check_slot_to_convoy );
                c.sp.setAttr( ATTR0_4BPP | ATTR0_SHAPE(0), ATTR1_SIZE_16 );
                c.sp.setTileId(16);

                c.spd = 2;
                c.dy = c.spd;

                convoys.push_back( Ship( c ) );
                timer_to_spawn_convoy = MAX_CONVOYS_TIMER_SPAWN;
            }
            check_slot_to_convoy++;
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
            if( convoys[i].pos.y > SCREEN_HEIGHT ){
                convoys[i].dead = TRUE;
                animEnemyExplode( &convoys[i] );
                if( convoys[i].sp.tid == ( 4 + 9 ) * 4 && convoys[i].dead ){
                    convoys[i].sp.hide();
                    convoys.erase( convoys.begin() + i );
                }
            }
        }else if( convoys[i].hp <= 0 ){
            destroyEnemy( &convoys[i], &convoys, i );
            if( !drop_item ){
                dropItem( convoys[i].pos.x, convoys[i].pos.y );
                drop_item = TRUE;
            }
        }else {
            convoys[i].sp.anim(16, 2);
            drop_item = FALSE;
        }

        if( convoys[i].damaged ){
            convoys[i].sp.pal = 1;
        }else {
            convoys[i].sp.pal = 0;
        }

        convoys[i].update();
    }

    updateItems();
}

void updateItems(){
    for( size_t i = 0; i < items.size(); i++ ){
        if( items[i].itemVsShip( &player ) ){
            itemIdValue( items[i].id );
            items[i].dead = TRUE;
            items[i].sp.hide();
            items.erase( items.begin() + i );
            return;
        }

        items[i].update();

        items[i].chase( player.pos.x, player.pos.y );
    }
}

void dropItem( int x, int y ){
    vu32 _rand_item = qran_range(ITEM_MEGA_BULLET, ITEM_IMORTAL);

    if( items.size() < MAX_ITEMS ){
        switch(_rand_item){
            case ITEM_MEGA_BULLET:
                dropItemType(x, y, 74, ITEM_MEGA_BULLET);
                break;
            case ITEM_POTION:
                dropItemType(x, y, 78, ITEM_POTION);
                break;
            case ITEM_MULTI_BULLET:
                dropItemType(x, y, 86, ITEM_MULTI_BULLET);
                break;
            case ITEM_BOOST_BULLET:
                dropItemType(x, y, 82, ITEM_BOOST_BULLET);
                break;
            case ITEM_IMORTAL:
                dropItemType(x, y, 90, ITEM_IMORTAL);
                break;
        }
        check_slot_to_item++;
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
