#ifndef __DEFS_H__
#define __DEFS_H__

#define GAME_NAME   "easyTetris"

#define SCREEN_WIDTH    305
#define SCREEN_HEIGHT   385

#define BOX_COLUMNS     10
#define BOX_ROWS        20
#define CELL_SIZE       16
#define MASK_SIZE       4
#define POSITION_NEXT   0xff
#define POSITION_HOLD   0xfe

#define BOX_LEFT        32
#define BOX_TOP         32

#define DEFAULT_MOVE_DELAY              1000
#define REDUCTION_MOVE_DELAY_PER_LEVEL  33
#define MINIMUM_MOVE_DELAY              50

#define NO_MASK     0xff

#define SCORE_PER_PIECE     10
#define SCORE_PER_LINE      (BOX_COLUMNS * SCORE_PER_PIECE)

#define LABEL_SCORE_TOP     279
#define LABEL_SCORE_LEFT    208

#define LABEL_LEVEL_TOP     (LABEL_SCORE_TOP + 24)
#define LABEL_LEVEL_LEFT    LABEL_SCORE_LEFT

#define LABEL_LINES_TOP     (LABEL_LEVEL_TOP + 24)
#define LABEL_LINES_LEFT    LABEL_SCORE_LEFT

#define LABEL_NEXT_TOP      BOX_TOP
#define LABEL_NEXT_LEFT     (BOX_LEFT + BOX_COLUMNS * CELL_SIZE + CELL_SIZE)

#define LABEL_HOLD_TOP      (BOX_TOP + (MASK_SIZE * CELL_SIZE) + BOX_TOP)
#define LABEL_HOLD_LEFT     (BOX_LEFT + BOX_COLUMNS * CELL_SIZE + CELL_SIZE)

#define LEFT_NEXT       LABEL_NEXT_LEFT
#define TOP_NEXT        (LABEL_NEXT_TOP + 16)

#define LEFT_HOLD       LABEL_HOLD_LEFT
#define TOP_HOLD        (LABEL_HOLD_TOP + 16)

#define MAX_PIECES  7

#endif  // __DEFS_H__