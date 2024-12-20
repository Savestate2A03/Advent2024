#define P1_SIZE 130
#define PMINI_SIZE 10

char* PUZZLEMINI[] = {
    "....#.....",
    ".........#",
    "..........",
    "..#.......",
    ".......#..",
    "..........",
    ".#..^.....",
    "........#.",
    "#.........",
    "......#..."
};

char* PUZZLE1[] = {
    "..................#................................................................#........#.....................................",
    "...#...........#...................................................#........................................#.................#...",
    "...................................#................#.#...............#.................................................#.........",
    ".....#......#................#.....................................................................#..........#........#......#...",
    "............................................................................................#.....................#...............",
    ".....................#..##...#........................#.................#.......................#..#.........#.......#...#......#.",
    "............##....................................##..................#...............................#....#......................",
    ".............#............#.#..#.......#...........#..............#...............#.....#.........................................",
    "....................#..........##.........#.........#................#............................................................",
    "....#.......#................................................................#...#.#..........................................#.#.",
    "..#..............#.....................................#..........#.#....#..............#..................#.#............#.......",
    ".#.......#.........................#........................#..#.....#............................................................",
    "........#...#......................#...........#......................................#....................#......................",
    ".............................#..............#........##....#....................#......#....................#............#......#.",
    "..............#..............#............#.......................##....#..........................................#.........#....",
    ".............#............#..........#..........#...#.....................................................#.......................",
    "..#........#.....#..........................................#................................................#...#.#...#..........",
    "........#.....................#...#..#......#.........................................................#.....##.........#..........",
    "..............................................................................................#...#...............................",
    "..#.......................#..........................#......#...................................................................#.",
    ".......#.........................#..............#.........#.............#.......................................#...#.............",
    "..................#.................#.......................#....................#....................................#...........",
    "...#...#.......#......##.#...............#....#..............#..........................................#....#..#.................",
    "#................................#.......#.....#.......#.............#..................#........#................................",
    "...........#..................................#.......................................................................#.#..#......",
    ".......#........#.........................................#..........................#...........................#.....#..........",
    "................#..#.....................#..#...........................#...........................................#.............",
    "#...#.......................#................................................#................#................................#..",
    "..........#...........................#.......#............................................................................#.....#",
    "..................................#............................................................#..................................",
    "......................#........#.............#....#.....#.......#..........................#..........................#...........",
    "...........................##.................#.............#........................#...#..................#.....................",
    ".............................................................................................................#....#..........#....",
    "................#...................................#...........#..........#....................#............................#....",
    "........#....#................................................#................##..#..................................#...........",
    "...........#....................................................#...#.#......#....................................................",
    ".......................#.........................................................^....................................#...........",
    "..........................#.............##..#........#.#....#.......#....................#...........#.........#.....#............",
    "........#...........................#..#..........................................................#...............................",
    "....#....................................#....#.........................................#.........#...............................",
    "...............#...................................#.....#.................................#....................................#.",
    "......#...............................................................#.............................#........................#....",
    ".........................#.....#.........................#.#...............#.........#.....#..................#..#.........#......",
    ".........................................................#......#........##.......................#...#......#................#...",
    ".......................................#.....#.................................#..................................................",
    "..................#..................................#..........................................................................#.",
    "...................#....#.........#.......#................#....................#.........................#......#.....#..#.......",
    "................................#..............................................................................#....#.............",
    ".....................#.#.............#.................#..........#.......#.........................................#.............",
    "......#....................#....................................................................#...#.............................",
    "........#........#...........#.##.............#........................#............#..............#.........#....#........#......",
    "........................#.................................................................#.......................................",
    "............................................................#.....#....................#............#.....#....#............#.....",
    "...........#............#....#........................................................##............#..............#..............",
    "...........#...........#.....#..............#..............##....#........#......#................................................",
    "..#................................................................................#......................#......#................",
    "....................#.........#......................#.............................#.......#.......................#.............#",
    "..#...#....#......................................................................................................#...............",
    "...........#................................................................................#...............................#.....",
    "....................................#............#..........#.........................#..................................#.......#",
    "...................#......#...................#............#......#........#................#.............#...........#.........#.",
    "........................#..................................#........#..................#..........................................",
    "............#..#...........#..................##....................#...........................................#.................",
    ".......................................................................................#.....#............#.......................",
    "...............................................#....#................................#..#..........................#..#...........",
    "..............#..#.....#.........#....................#.......#..#..............#...............................#.....#...........",
    "..........#............................#..........................................................................................",
    "........................................#........#........................................................#.#.....................",
    "#................#......................#..................................................................................#......",
    "...............................................................................#................................#.................",
    "...................................................................................................................#.#........#...",
    "#........................................................................#......................................#.#...............",
    "...#......#............#...................#.............#........#..........................................#......#.............",
    "..............##....#...................................................#......................#..................................",
    "...............#........................................................................#...........#...#.#.......................",
    "..........#.#.....#................................#...........#........#.......#.................................................",
    "#....................#......................................................................................#.....................",
    "..............#...##...........................................#.............#.......#..................#..............#..........",
    ".#.............................................#..............................................................................#...",
    "...#................................#............................................#............................#...................",
    "..............................#......................................................#..........................................#.",
    "...................#.....................................#..................................#......#..............................",
    "....#.#..........##.............#.......#........................##..........#.................#..................................",
    "#..#................#........##....................................................................................#..............",
    "........#...#.........................................................................#...........................................",
    "......................................................#....#...................#..#.................................#.............",
    "........#...............#........#..............................................#............................#....................",
    "...#..............................#.........................................##.......................................#....#.......",
    "...#................#..............#.......#......................................................................................",
    "........#..................#.............................................................#...........................#....#.......",
    "......................#.#..........#.......#..................................#..#...#............#...............#...............",
    ".....#..................#....#..#......#.......................#.....................................................#............",
    ".....#................................#.........................#..................#.#..........#...#............#.....#..........",
    "....#...................#....#.#.........#........#..................................................................#....#..#....",
    "........#...............#......................................#........#..............................#..........................",
    "....#...#.#.....#..............#..................#..........#...#..#......................#...#..........#......................#",
    "...........#...................................................................................................#................#.",
    ".................................##..............#.............................#...............................#..................",
    "...........#.................#.............##...................................#.......................#.................##......",
    ".....#..................#......#........#.................................................#..#........................#..#........",
    "......#.......#......................................................................#.#.........#....#...........................",
    "..................#.#........................#......#................................................................#......#.....",
    ".....................#............#...................#.....#....................................#.......#..........#.......#.....",
    "#.............................#...............................#...#...............................................................",
    "..##.........#........#......................................#.....................#...#.............#............#..#............",
    "..........#.....................#..#....................#...#....................#....................#.....................#.....",
    "...........#..........#...............#..#..........#..............................................#.#............................",
    "............#...#...#.............#.........#.............................#...............#.......................................",
    ".........#...........#...............................................................#............................................",
    ".#..#.................................................................#............#.#.......................##...................",
    "........##............#...................#................................#......................................................",
    "............................................#..........................#..........................................................",
    "#...................#...........#..............#...#............#.#........#.................#......#.............................",
    "........#....................................#....................................................................................",
    "...........#.........#..#...............#...........#.....##.#......#....##............#....#....#.#.........##.........#.........",
    "..#......................#..........#.................#.................#......#.............................#........#...........",
    "....................#.................#....#..............................#....#.............#....#....#.....................#....",
    "............#.................#.................................#...............................#..#...........................#..",
    "...................................................#.....#..............#.......#....................#.#..........................",
    "..#.........##..#........#...................................................................................#............#.......",
    ".....#..............................#......................................................................#........#.............",
    "................................#........##.......#...............#..............#.......#...#.#........#.........................",
    "..............................#.....................#.........................................#..................................#",
    "...........#...................##.................#.................................#...............................#........#....",
    "..............................#...##......#..................#...#................................#......#.......#...............#",
    ".....................##......#......#.#.......#..............#......#.................#.........#...................#.............",
    "..........................#................................#...........#..........................................................",
    "...#.....#....................#.....................#...#.....#.............................#.#....#.....#.#.................#....",
    "............................#...#........#......................................................................#.....#...........",
    "............#..........##..................#.............................................................#.....#..#..............."
};