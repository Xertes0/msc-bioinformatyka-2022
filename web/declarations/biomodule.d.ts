declare class BioModule {
    test(): number;
    translate(sequence: string, callback: (id: number) => [string, string]): [];
    drawSkeletal(sequence: string): string;
}

declare module "@cxx/biolib/bio.mjs" {
    export default function BioModuleLoad(): Promise<BioModule>;
}

