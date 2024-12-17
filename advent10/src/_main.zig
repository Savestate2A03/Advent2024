const std = @import("std");
const ArrayList = std.ArrayList;

const map_data = [_][]const u8{ "89010123", "78121874", "87430965", "96549874", "45678903", "32019012", "01329801", "10456732" };

pub fn Trail() type {
    return struct {
        const Self: type = @This();
        const Node: type = struct {
            x: i32 = 0, y: i32 = 0, height: i32 = 0,
            index: usize = undefined, parent: usize = undefined };
        const UCoords: type = struct { x: i32, y: i32 };
        const GeneratedNodes: type = struct { nodes: [4]Node = .{.{}, .{}, .{}, .{}}, length: usize = 0 };
        
        nodes: ArrayList(Node),
        map: []const []const u8,
        allocator: std.mem.Allocator,
        paths: ArrayList(Node),

        pub fn charToInt(character: u8) i32 {
            return character - 48;
        }

        pub fn init(allocator: std.mem.Allocator, map: []const []const u8) Self {
            return Self{
                .allocator = allocator,
                .paths = ArrayList(Self.Node).init(allocator),
                .nodes = ArrayList(Self.Node).init(allocator),
                .map = map
            };
        }

        pub fn deinit(self: Self) void {
            self.nodes.deinit();
            self.paths.deinit();
        }

        pub fn generate_valid_nodes(self: Self, node: Node) GeneratedNodes {
            var next_nodes = GeneratedNodes{ .length = 0};
            const spots = [_]UCoords{
                UCoords{.x = node.x,     .y = node.y - 1},
                UCoords{.x = node.x,     .y = node.y + 1},
                UCoords{.x = node.x + 1, .y = node.y},
                UCoords{.x = node.x - 1, .y = node.y}
            };
            for (0.., spots) |i, spot| {
                const uX: u32 = @bitCast(spot.x);
                const uY: u32 = @bitCast(spot.y);
                if (spot.y < self.map.len and spot.y >= 0
                    and spot.x < self.map[uY].len and spot.x >= 0) {
                    std.debug.print("looking at node height: {d}, current height is: {d}\n", .{charToInt(self.map[uY][uX]), node.height});
                    if (charToInt(self.map[uY][uX]) == node.height + 1) {
                        next_nodes.nodes[i] = Node{
                            .x = spot.x,
                            .y = spot.y,
                            .height = node.height + 1,
                            .parent = node.index,
                            .index = self.map.len
                        };
                        next_nodes.length += 1;
                        std.debug.print("added at height {d}!\n", .{next_nodes.nodes[i].height});
                    }
                }
            }
            return next_nodes;
        }

        pub fn gather_trails(self: *Self) !void {
            while (true) {
                const trail = self.nodes.pop();
                std.debug.print("gathering node of height: {d}\n", .{trail.height});
                if (trail.height != 9) break;
                try self.paths.append(trail);
            }
        }

        pub fn explore_graph(self: *Self) !void {
            var index: usize = 0;
            while (index < self.nodes.items.len) {
                std.debug.print("nodes length: {d}\n", .{self.nodes.items.len});
                const next_nodes = self.generate_valid_nodes(self.nodes.items[index]);
                try self.nodes.appendSlice(next_nodes.nodes[0..next_nodes.length]);
                index += 1;
            }
        }

        pub fn build_nodes(self: *Self) !void {
            var index: usize = 0;
            for (0.., self.map) |y, row| {
                for (0.., row) |x, digit| {
                    if (digit == '0') {
                        try self.nodes.append(Node{ .x = @intCast(x), .y = @intCast(y), .index = index});
                        index += 0;
                    }
                }
            }
        }

        pub fn trailheads(self: Self) void {
            for (self.nodes.items) |node| {
                std.debug.print("({d}, {d})\n", .{ node.x, node.y });
            }
        }
    };
}

pub fn main() !void {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer if (gpa.deinit() == .leak) unreachable;

    const allocator = gpa.allocator();

    var trail = Trail().init(allocator, &map_data);
    defer trail.deinit();

    try trail.build_nodes();
    try trail.explore_graph();
    try trail.gather_trails();
    std.debug.print("trails: {d}\n", .{trail.paths.items.len});
}