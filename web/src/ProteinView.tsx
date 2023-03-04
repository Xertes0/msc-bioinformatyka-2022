import React, { SyntheticEvent, useEffect, useState } from "react";
import BioModuleLoad from "@cxx/biolib/bio.mjs";
import "./ProteinView.css"
import { data } from "./AminoAcidData";


export interface FoundProteinsViewProps {
  sequence: string;
  onProteinClick(event: OpenReadingFrameClick): void;
}

export interface OpenReadingFrameClick extends SyntheticEvent<HTMLTableRowElement> {
  target: HTMLTableRowElement;
  sequence: string;
}

export class OpenReadingFrame {
  nbStart?: number;
  nbEnd?: number;
  nbShift?: 0 | 1 | 2 | undefined;
  aaStart?: number;
  aaEnd?: number;
}

function ProteinView({ sequence, onProteinClick }: FoundProteinsViewProps) {

  const [bioModule, setBioModule] = useState<null | BioModule>(null);

  useEffect(() => {
    BioModuleLoad().then((res) => {
      setBioModule(res);
    })
  }, []);

  const result = bioModule?.translate(sequence);

  const translatedSequence: [string, string, string] =
    result?.map((v) => v.sequence) as [string, string, string] || []

  const openReadingFrames =
    bioModule?.translate(sequence)?.map((result, resultIdx) => {
      let indices: OpenReadingFrame[] = [];
      for (let i = 0; i < result.indices.size(); i++) {
        const {end, start} = result.indices.get(i);
        indices[i] = Object.assign(new OpenReadingFrame(), {
          nbStart: start * 3 + resultIdx,
          nbEnd: end * 3 + resultIdx,
          nbShift: resultIdx,
          aaStart: start,
          aaEnd: end,
        });
      }
      return indices;
    }) as [OpenReadingFrame[], OpenReadingFrame[], OpenReadingFrame[]] || [];

  const openReadingFramesCount = openReadingFrames?.flat().reduce(r => r + 1, 0);

  const higlightColors = Array.from(
    { length: openReadingFramesCount },
    (_, i) => `hsl(${i / openReadingFramesCount * 360 + 60}, 100%, 50%)`
  );

  const [activeOpenReadingFrame, setActiveOpenReadingFrame] = useState<undefined | OpenReadingFrame>(undefined);

  let orfs = -1;
  return bioModule && openReadingFrames ?
    <table className="protein-view">
      <thead>
        <tr className="nb-seq header">
          { Array.from(sequence).map((c, ci) => <th key={ci} className="nb" data-idx={ci}>{c}</th>) }
        </tr>
      </thead>
      <tbody>
        {
          translatedSequence.map((seq, seqIdx) =>
            (
              <>
                <tr key={`seq-${seqIdx}`} className="aa-seq header" data-shift={seqIdx} data-seq={seq}>
                  {seqIdx > 0 && <th colSpan={seqIdx}></th>}
                  {
                    Array.from(seq).map((aa, aaIdx) =>
                      <th key={aaIdx} colSpan={3} className="aa" data-idx={aaIdx}>
                        {data.find(aad => aad.symbol == aa)?.abbreviation || "-"}
                      </th>)
                  }
                </tr>
                {openReadingFrames[seqIdx].map((orf, orfIdx) => {
                  orfs++;
                  return (
                    <tr key={`orf-${orfIdx}`}
                        className={`orf${
                          activeOpenReadingFrame?.nbStart == orf.nbStart &&
                          activeOpenReadingFrame?.nbEnd == orf.nbEnd
                            ? " active" : ""
                        }`}
                        data-shift={seqIdx}
                        data-orf={JSON.stringify(orf)}>
                      {(orf.nbStart || 0) > 0 && <td colSpan={orf.nbStart}></td>}
                      {Array.from(seq.slice(orf.aaStart, orf.aaEnd)).map((aa, aaIdx) =>
                        <td key={aaIdx}
                            colSpan={3}
                            className="aa"
                            data-idx={aaIdx}
                            style={{ borderColor: higlightColors[orfs] }}>{data.find(aad => aad.symbol == aa)?.abbreviation}</td>
                      )}
                    </tr>
                  );
                })}
              </>
            ))
        }
      </tbody>
    </table> :
    <div>Loading...</div>
}

export default ProteinView;