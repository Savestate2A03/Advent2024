const std = @import("std");
const ArrayList = std.ArrayListUnmanaged;

/// NodeProcessor generates a node tree based on arguments that are given to its
/// initalization functions (init/init_with). It also provides functions for the
/// NodeProcessor to print paths from each leaf to its root node, and from each
/// root node to its leaf nodes.
pub fn NodeProcessor() type {
    return struct {
        const Self: type = @This();
        const NodeIndex: type = usize;
        const Node: type = struct {
            value: i32 = 0,
            parent: NodeIndex = 0,
            children: ArrayList(NodeIndex),
            root: bool = false,
            leaf: bool = false
        };
        
        nodes: ArrayList(Node),
        roots: ArrayList(NodeIndex),
        leaves: ArrayList(NodeIndex),
        allocator: std.mem.Allocator,
        prng: std.Random.Xoshiro256,
        depth: usize,         // maximum depth of the generated node tree
        children_odds: usize, // the max number of children that a node can get
        randomize: bool,      // if randomize is false, each node will generate exactly children_odds children

        /// Sets up a new NodeProcessor by preparing ArrayLists and the psuedo-random
        /// number generator. Defaults are provided for a few things which can be
        /// overridden with self.init_with(...)
        pub fn init(allocator: std.mem.Allocator) Self {
            return Self{
                .allocator = allocator,
                .nodes = ArrayList(Node).empty,
                .leaves = ArrayList(NodeIndex).empty,
                .roots = ArrayList(NodeIndex).empty,
                .prng = std.Random.DefaultPrng.init(@bitCast(std.time.milliTimestamp())),
                .depth = 4,
                .children_odds = 4,
                .randomize = true
            };
        }

        /// Allow overriding depth, odds, and whether or not to randomize children generated
        pub fn init_with(allocator: std.mem.Allocator, depth: usize, odds: usize, randomize: bool) Self {
            var self = Self.init(allocator);
            self.depth = depth;
            self.children_odds = odds;
            self.randomize = randomize;
            return self;
        }

        pub fn deinit(self: *Self) void {
            for (0..self.nodes.items.len) |node| {
                self.nodes.items[node].children.deinit(self.allocator);
            }
            self.nodes.deinit(self.allocator);
            self.leaves.deinit(self.allocator);
            self.roots.deinit(self.allocator);
        }

        /// Generates 'count' number of root nodes. Stores the root node indexes
        /// in the .roots ArrayList for quick lookup later.
        pub fn init_nodes(self: *Self, count: usize) !void {
            try self.nodes.appendNTimes(self.allocator, Node{
                    .root = true,
                    .children = ArrayList(NodeIndex).empty
                }, count
            );
            for (0..self.nodes.items.len) |root_index| {
                try self.roots.append(self.allocator, root_index);
            }
        }

        /// Assuming our depth is not met, generates a number of children nodes
        /// and adds them to our node list while adding the childrens' indexes to
        /// the parent node's children ArrayList.
        pub fn create_child_nodes(self: *Self, index: NodeIndex) !void {
            var node: *Node = &self.nodes.items[index];
            const children_count: usize = if (self.randomize) self.prng.next() % self.children_odds else self.children_odds;
            if (!(node.value < self.depth) or children_count == 0) {
                try self.leaves.append(self.allocator, index);
                node.leaf = true;
                return;
            }
            const child_node: Node = Node{
                .value = node.value + 1,
                .parent = index,
                .children = ArrayList(NodeIndex).empty
            };
            for (0..children_count) |child| {
                try node.children.append(self.allocator, self.nodes.items.len + child);
            }
            try self.nodes.appendNTimes(self.allocator, child_node, children_count);
        }

        /// Kicks off the children node generation. Until we run out of nodes,
        /// keep generating them based on the configuration of the NodeProcessor
        pub fn sprawl(self: *Self) !void {
            var index: NodeIndex = 0;
            // while loop instead of for loop since a for loop will
            // only use the initial value of self.nodes.items.len. it
            // needs to be reevaluated every loop.
            while(index < self.nodes.items.len) {
                try self.create_child_nodes(index);
                index += 1;
            }
        }

        /// Using our leaf indexes, follow the parent chain up and print them
        /// out until a root node is reached.
        pub fn print_leaves_to_roots(self: *Self) !void {
            var path = ArrayList(NodeIndex).empty;
            const leaves = self.leaves.items;
            const nodes = self.nodes.items;

            var node: *Node = undefined;
            for (leaves) |leaf| {
                // start the index path with the leaf itself
                try path.append(self.allocator, leaf);
                node = &nodes[leaf];
                // for each leaf, append its parent index
                while (!node.root) {
                    try path.append(self.allocator, node.parent);
                    node = &nodes[node.parent];
                }
                // "(leaf) leaf -> parent -> parent -> ... -> root (root)"
                std.debug.print("(leaf)", .{});
                for (path.items) |step| {
                    const arrow = if (step != leaf) " ->" else "";
                    std.debug.print("{s} {d}", .{arrow, step});
                }
                std.debug.print(" (root)\n", .{});
                path.clearAndFree(self.allocator); // prepare path index array for next leaf
            }
        }

        /// Given a node, print its children recursively with a DFS
        pub fn print_children(self: *Self, index: NodeIndex, nodestack: *ArrayList(NodeIndex)) !void {
            // push a given node to a given stack representing a node path
            var node: *Node = &self.nodes.items[index];
            try nodestack.append(self.allocator, index);
            if (!node.leaf) {
                // if the node is not the last in the search, recursively add it to the node path stack
                for (node.children.items) |child| {
                    try self.print_children(child, nodestack);
                }
            } else {
                // once we have reached the end of a node path, print it out
                std.debug.print("(root)", .{});
                for (nodestack.items) |step| {
                    node = &self.nodes.items[step];
                    const arrow = if (!node.root) " ->" else "";
                    std.debug.print("{s} {d}", .{arrow, step});
                }
                std.debug.print(" (leaf)\n", .{});
            }
            // pop the last node from the stack once it has finally
            // resolved to restore the previous node's context
            _ = nodestack.pop();
        }

        /// Prints every path to each leaf for every root node
        pub fn print_roots_to_leaves(self: *Self) !void {
            var nodestack = ArrayList(NodeIndex).empty;
            defer nodestack.deinit(self.allocator);

            for (self.roots.items) |root| {
                try self.print_children(root, &nodestack);
            }
        }
    };
}

pub fn main() !void {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer if (gpa.deinit() == .leak) unreachable;

    const allocator = gpa.allocator();

    var processor = NodeProcessor().init_with(allocator, 5, 6, true);
    defer processor.deinit();

    try processor.init_nodes(4);
    try processor.sprawl();
    try processor.print_leaves_to_roots();
    try processor.print_roots_to_leaves();
}