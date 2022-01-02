#include "gamelogic.h"

struct CooldownTimers {
    double x1, x2,
    y1, y2,
    r1, r2;
};

struct FrametimeLimitTimers {
    double t1, t2;
    double work_time;
    double sleep_time;
};

int32_t mainloop(struct Buffer *console)
{
    // Clear screen with space characters
    memset(console->buffer, ' ', console->size_in_bytes);
    
    // Play field frame drawing to console buffer
    const vec2i start_land_frame_coord = { .x = 0, .y = 0 };
    const vec2i end_land_frame_coord = { .x = 11, .y = 17 };
    draw_frame(console, start_land_frame_coord, end_land_frame_coord);
    
    // Next tetromino preview frame drawing to console buffer
    const vec2i start_preview_frame_coord = { .x = 13, .y = 0 };
    const vec2i end_preview_frame_coord = { .x = 18, .y = 5 };
    draw_frame(console, start_preview_frame_coord, end_preview_frame_coord);
    
    // State buffer of falling tetromino and landed blocks
    char field_buffer[10*16] = {0};
    struct Buffer field = {
        .buffer = field_buffer,
        .width = 10,
        .height = 16,
    };
    
    // landed blocks with field 10x16 (10 columns and 16 rows)
    char landed_buffer[10*16] = {0};
    struct Buffer landed = {
        .buffer = landed_buffer,
        .width = 10,
        .height = 16,
        .size_in_bytes = 10*16
    };
    
    struct Tetromino tetrominoes[7] = {0};
    init_tetrominoes(tetrominoes);
    
    // setting up timers
    struct CooldownTimers cd;
    cd.x1 = get_time_in_seconds();
    cd.y1 = get_time_in_seconds();
    cd.r1 = get_time_in_seconds();
    
    // choose current tetromino
    int32_t next_tetromino = rand() % 7;
    // randomly choose the next value for next loop iteration
    int32_t current_tetromino = rand() % 7;
    
    // cooldowns
    double falling_cooldown = 0.5;
    const double movement_cooldown = 0.17;
    const double rotate_cooldown = 0.2;
    
    int32_t score = 0;
    
    // setting up fps limit timers
    struct FrametimeLimitTimers fl = {
        .t1 = get_time_in_seconds(),
        .t2 = get_time_in_seconds(),
        .work_time = 0,
        .sleep_time = 0
    };
    
    // main game loop
    for (;;)
    {
        
        // FPS limit
        fl.t1 = get_time_in_seconds();
        
        fl.work_time = time_diff(fl.t2, fl.t1);
        if (fl.work_time < 16.6/1000.0)
        {
            unsigned int sleep_time = (unsigned int)(16-fl.work_time);
            platform_sleep(sleep_time);
        }
        
        fl.t2 = get_time_in_seconds();
        fl.sleep_time = time_diff(fl.t1, fl.t2);
        
        struct Keys keys = {0};
        
        // input
        get_key(&keys);
        
        if (keys.escape)
        {
            break;
        }
        
        cd.y2 = get_time_in_seconds();
        // increased falling
        if (keys.down && time_diff(cd.y1, cd.y2) > (falling_cooldown / 5.0))
        {
            cd.y1 = get_time_in_seconds();
            ++tetrominoes[current_tetromino].potentialTopLeft.y;
        }
        // normal falling
        else if (time_diff(cd.y1, cd.y2) > falling_cooldown)
        {
            cd.y1 = get_time_in_seconds();
            ++tetrominoes[current_tetromino].potentialTopLeft.y;
        }
        
        cd.x2 = get_time_in_seconds();
        if (keys.left && time_diff(cd.x1, cd.x2) > movement_cooldown)
        {
            cd.x1 = get_time_in_seconds();
            --tetrominoes[current_tetromino].potentialTopLeft.x;
        }
        else if (keys.right && time_diff(cd.x1, cd.x2) > movement_cooldown)
        {
            cd.x1 = get_time_in_seconds();
            ++tetrominoes[current_tetromino].potentialTopLeft.x;
        }
        
        cd.r2 = get_time_in_seconds();
        if (keys.up && time_diff(cd.r1, cd.r2) > rotate_cooldown)
        {
            cd.r1 = get_time_in_seconds();
            rotate_tetromino(&tetrominoes[current_tetromino], &landed);
        }
        
        if (check_colision(&tetrominoes[current_tetromino], &landed))
        {
            // if row filled increasing score and falling down speed 
            check_filled_row(&landed, &score, &falling_cooldown);
            if (game_over_check(&tetrominoes[next_tetromino], &landed))
            {
                break;
            }
            
            current_tetromino = next_tetromino;
            next_tetromino = rand() % 7;
        }
        else
        {
            tetrominoes[current_tetromino].topLeft = tetrominoes[current_tetromino].potentialTopLeft;
        }
        
        place_landed_blocks_to_field(&field, &landed);
        place_tetromino_to_field(&field, &tetrominoes[current_tetromino]);
        
        const vec2i tetromino_preview_coord = {
            .x = start_preview_frame_coord.x+1,
            .y = start_preview_frame_coord.y+1
        };
        draw_next_tetromino_preview(console, &tetrominoes[next_tetromino], tetromino_preview_coord);
        
        const vec2i score_pos = {
            .x = start_preview_frame_coord.x,
            .y = start_preview_frame_coord.y+6,
        };
        draw_score(console, score_pos, score);
        
        const vec2i field_coord = { .x = 1, .y = 1 };
        draw_field(console, &field, field_coord);
        
        // Copy screen buffer to console buffer
        write_to_console(console);
    }
    
    // Copy last frame to console buffer
    write_to_console(console);
    
    // Final score print
    const vec2i final_score_pos = {.x = 0, .y = 0};
    clear_screen_with(console, ' ');
    draw_score(console, final_score_pos, score);
    platform_sleep(1000);
    
    // return score for console printing
    return score;
}