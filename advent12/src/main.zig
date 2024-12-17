const std = @import("std");
const Array = std.ArrayListUnmanaged;
const GPA = std.heap.GeneralPurposeAllocator;
const Allocator = std.mem.Allocator;

pub fn ClawGame() type {
  return struct {
    const ClawSelf: type = @This();
    const Coordinates: type = struct { x: usize = 0, y: usize = 0 };
    const Parameters: type = struct {
      a: Coordinates = .{},
      b: Coordinates = .{},
      prize: Coordinates = .{}
    };
    const ACost: usize = 3;
    const BCost: usize = 1;

    allocator: Allocator,
    params: Parameters,
    wnt: WeightedNodeTree(),
  
    pub fn init(allocator: Allocator, params: Parameters) ClawSelf {
      return ClawSelf{
        .allocator = allocator,
        .params = params,
        .wnt = WeightedNodeTree().init()
      };
    }

    pub fn start(self: *ClawSelf) !void {
      try self.wnt.start(self);
    }

    pub fn deinit(self: *ClawSelf) void {
      self.wnt.deinit();
    }

    fn WeightedNodeTree() type {
      return struct {
        const WNTSelf: type = @This();
        const NodeIndex: type = usize;
        const Order = std.math.Order;
        const NodePriorityQueue: type = std.PriorityQueue(NodeIndex, *WNTSelf, node_compare);
        const Node: type = struct {
          parent: NodeIndex,
          children: Array(NodeIndex) = Array(NodeIndex).empty,
          coords: Coordinates,
          weight: usize,
          dist_scalar: usize = 0,
        };

        known: Array(Coordinates),
        nodes: Array(Node),
        queue: NodePriorityQueue,
        parent: *ClawSelf,

        pub fn known_coordinates(self: *WNTSelf, coords: Coordinates) bool {
          for (self.known.items) |known| {
            if (std.meta.eql(coords, known)) return true;
          }
          return false;
        }

        pub fn add_children(self: *WNTSelf, node: NodeIndex) !void {
          const coords: Coordinates = self.nodes.items[node].coords;
          const weight: usize = self.nodes.items[node].weight;
          const prize: Coordinates = self.parent.params.prize;
          const node_a_x = coords.x + self.parent.params.a.x;
          const node_a_y = coords.y + self.parent.params.a.y;
          if (!(node_a_x <= prize.x and node_a_y <= prize.y)) return;
          const node_a: Node = Node{
            .coords = .{
              .x = node_a_x,
              .y = node_a_y
            },
            .weight = weight + ACost,
            .parent = node,
            .dist_scalar = (prize.x - node_a_x)*(prize.x - node_a_x) + (prize.y - node_a_y)*(prize.y - node_a_y)
          };
          const node_b_x = coords.x + self.parent.params.b.x;
          const node_b_y = coords.y + self.parent.params.b.y;
          if (!(node_b_x <= prize.x and node_b_y <= prize.y)) return;
          const node_b: Node = Node{
            .coords = .{
              .x = node_b_x,
              .y = node_b_y
            },
            .weight = weight + BCost,
            .parent = node,
            .dist_scalar = (prize.x - node_b_x)*(prize.x - node_b_x) + (prize.y - node_b_y)*(prize.y - node_b_y)
          };
          const only_b: bool = (node_b.dist_scalar < node_a.dist_scalar);
          for (&[_]Node{node_a, node_b}) |child| {
            if (only_b) continue;
            if (!self.known_coordinates(child.coords) and child.dist_scalar < self.nodes.items[node].dist_scalar) {
              try self.nodes.items[node].children.append(self.parent.allocator, self.nodes.items.len);
              try self.nodes.append(self.parent.allocator, child);
              try self.queue.add(self.nodes.items.len-1);
              try self.known.append(self.parent.allocator, child.coords);
            }
          }
        }

        pub fn explore(self: *WNTSelf) !void {
          var node: NodeIndex = undefined;
          var queue_max_size: usize = 0;
          while (self.queue.count() > 0) {
            node = self.queue.remove();
            try self.add_children(node);
            const queue_count = self.queue.count();
            if (queue_count > queue_max_size) queue_max_size = queue_count;
            // std.debug.print("({d}. {d}) - Weight: {d}\n", .{
            //   self.nodes.items[node].coords.x,
            //   self.nodes.items[node].coords.y,
            //   self.nodes.items[node].weight
            // });
          }
          if (std.meta.eql(self.nodes.items[node].coords, self.parent.params.prize)) {
            std.debug.print("Prize found!\nFinal node stats! Weight: {d}, Position: ({d},{d})\n",.{
              self.nodes.items[node].weight,
              self.nodes.items[node].coords.x,
              self.nodes.items[node].coords.y,
            });
          } else {
            std.debug.print("Prize not found...\nFinal node stats! Weight: {d}, Position: ({d},{d})\n",.{
              self.nodes.items[node].weight,
              self.nodes.items[node].coords.x,
              self.nodes.items[node].coords.y,
            });
          }
          std.debug.print("Stats! Queue max size: {d}, Nodes created: {d}\n", .{queue_max_size, self.nodes.items.len});
        }

        pub fn generate_root_node(self: *WNTSelf) !void {
          const prize: Coordinates = self.parent.params.prize;
          try self.nodes.append(self.parent.allocator, Node{
            .coords = .{.x = 0, .y = 0},
            .weight = 0,
            .parent = undefined,
            .dist_scalar = (prize.x)*(prize.x) + (prize.y)*(prize.y)
          });
          try self.queue.add(self.nodes.items.len-1);
          try self.known.append(self.parent.allocator, Coordinates{.x = 0, .y = 0});
        }

        pub fn node_compare(self: *WNTSelf, a: NodeIndex, b: NodeIndex) Order {
          const nA = self.nodes.items[a];
          const nB = self.nodes.items[b];
          if (nA.weight < nB.weight)  return Order.lt;
          if (nA.weight > nB.weight)  return Order.gt;
          const cA = nA.coords;
          const cB = nB.coords;
          const cP = self.parent.params.prize;
          const distA = (cP.x - cA.x)*(cP.x - cA.x) + (cP.y - cA.y)*(cP.y - cA.y);
          const distB = (cP.x - cB.x)*(cP.x - cB.x) + (cP.y - cB.y)*(cP.y - cB.y);
          if (distA < distB)  return Order.lt;
          if (distA > distB)  return Order.gt;
          if (distA == distB) return Order.eq;
          unreachable;
        }

        pub fn init() WNTSelf {
          return WNTSelf{
            .nodes = Array(Node).empty,
            .known = Array(Coordinates).empty,
            .parent = undefined,
            .queue = undefined
          };
        }

        pub fn start(self: *WNTSelf, parent: *ClawSelf) !void {
            self.parent = parent;
            self.queue = NodePriorityQueue.init(self.parent.allocator, self);
            try self.generate_root_node();
            try self.explore();
        }

        pub fn deinit(self: *WNTSelf) void {
          for (0..self.nodes.items.len) |i| {
            self.nodes.items[i].children.deinit(self.parent.allocator);
          }
          self.known.deinit(self.parent.allocator);
          self.nodes.deinit(self.parent.allocator);
          self.queue.deinit();
        }
      };
    }
  };
}

pub fn main() !void {
  var gpa = GPA(.{}){};
  defer if (gpa.deinit() == .leak) unreachable;
  const allocator = gpa.allocator();

  const params = ClawGame().Parameters{
    .a     = .{.x = 17,   .y = 55},
    .b     = .{.x = 54,   .y = 13},
    .prize = .{.x = 17532, .y = 19258}
  };

  var clawgame = ClawGame().init(allocator, params);
  defer clawgame.deinit();

  std.debug.print("Starting...\n", .{});
  try clawgame.start();
}

// test "simple test" {
//     var list = std.ArrayList(i32).init(std.testing.allocator);
//     defer list.deinit(); // Try commenting this out and see if zig detects the memory leak!
//     try list.append(42);
//     try std.testing.expectEqual(@as(i32, 42), list.pop());
// }

// test "fuzz example" {
//     const global = struct {
//         fn testOne(input: []const u8) anyerror!void {
//             // Try passing `--fuzz` to `zig build test` and see if it manages to fail this test case!
//             try std.testing.expect(!std.mem.eql(u8, "canyoufindme", input));
//         }
//     };
//     try std.testing.fuzz(global.testOne, .{});
// }
