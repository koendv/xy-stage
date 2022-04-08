## openscad

[![ disassembled.jpg ]( ../pictures/disassembled_small.jpg )]( ../pictures/disassembled.jpg )

These are the design files for the adapter plates. The adapter plates are designed using [openscad](http://openscad.org). There are two versions of the carriage, one with a hole for illumination from below and one without.

- [source file](stage.scad)
- [stage top, solid](stage1.stl)
- [stage top, with hole for illumination](stage2.stl)
- [stage bottom](stageplate.stl)
- [stage bottom, with bracket to attach arduino nano](stageplate_arduinonano.stl)
- [stage floorplan](stage.svg)

Two 3D printing methods have been used:

- FDM, PETG material, [Prusa i3](https://www.treatstock.com/c/seaside-3d).
- SLA, LEDO 6060 resin, [jlcpcb 3d](https://jlcpcb.com/3d-printing)

In the end, how to print depends upon price.

Some ideas for improvements:

- more stable carriage, larger hole for through illumination if the other side of the xy table is up
- smaller and lighter carriage if 18x18x1mm cover slips are used instead of 75x25mm slides.
- arduino nano holder