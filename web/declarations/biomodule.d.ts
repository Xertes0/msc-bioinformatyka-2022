declare class IndexPair {
    start: number;
    end:   number;
}

declare class IndexVector {
    size(): number;
    get(index): IndexPair;
};

declare class OrfResult {
    sequence: string;
    indices:  IndexVector;
}

declare class BioModule {
    test(): number;
    translate(sequence: string): [OrfResult, OrfResult, OrfResult];
    drawSkeletal(sequence: string): string;
}

declare module "@cxx/biolib/bio.mjs" {
    export default function BioModuleLoad(): Promise<BioModule>;
}

