const std = @import("std");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const exe = b.addExecutable(.{
        .name = "kjell",
        .target = target,
        .optimize = optimize,
    });

    exe.addCSourceFile(.{
        .file = .{ .cwd_relative = "src/main.c" },
        .flags = &[_][]const u8{
            "-std=c89",
            "-Wall",
            "-Wextra",
        },
    });

    exe.linkLibC();

    b.installArtifact(exe);

    const run_cmd = b.addRunArtifact(exe);
    run_cmd.step.dependOn(b.getInstallStep());

    const run_step = b.step("run", "Run the app");
    run_step.dependOn(&run_cmd.step);
}
