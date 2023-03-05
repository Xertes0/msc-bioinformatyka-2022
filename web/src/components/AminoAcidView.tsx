import React, { SyntheticEvent } from "react";
import "./AminoAcidView.css";
import OpenReadingFrame from "./OpenReadingFrame";

export interface AminoAcidViewProps {
  orfData: [
    OpenReadingFrameViewData[],
    OpenReadingFrameViewData[],
    OpenReadingFrameViewData[]
  ];
  selectedOrf: [number, number] | undefined;
  sequence: string[];
  sequences: [string[], string[], string[]];

  onOpenReadingFrameClick(event: OpenReadingFrameClick): void;
}

export interface OpenReadingFrameViewData {
  sequence: string[];
  shift: number;
}

export interface OpenReadingFrameClick
  extends SyntheticEvent<HTMLTableRowElement> {
  target: HTMLTableRowElement;
  idx: [number, number];
}

function AminoAcidView({
  orfData,
  selectedOrf,
  sequence,
  sequences,
  onOpenReadingFrameClick,
}: AminoAcidViewProps) {
  const openReadingFramesCount = orfData?.flat().reduce((r) => r + 1, 0);
  const highlightColors = Array.from(
    { length: openReadingFramesCount },
    (_, i) => `hsl(${(i / openReadingFramesCount) * 360 + 60}, 100%, 50%)`
  );

  let orfs = 0;
  return orfData ? (
    <table className="amino-acid-view">
      <thead>
        <tr className="nb-seq header">
          {sequence?.map((c, ci) => (
            <th key={ci} className="nb" data-idx={ci}>
              {c}
            </th>
          ))}
        </tr>
      </thead>
      <tbody>
        {sequences?.map((seq, seqIdx) => (
          <>
            <tr
              key={`seq-${seqIdx}`}
              className="aa-seq header"
              data-shift={seqIdx}
              data-seq={seq}
            >
              {seqIdx > 0 && <th colSpan={seqIdx}></th>}
              {Array.from(seq).map((aa, aaIdx) => (
                <th key={aaIdx} colSpan={3} className="aa" data-idx={aaIdx}>
                  {aa}
                </th>
              ))}
            </tr>
            {orfData[seqIdx].map((orf, orfIdx) => {
              const index: [number, number] = [seqIdx, orfIdx];
              let tr = (
                <OpenReadingFrame
                  color={highlightColors[orfs]}
                  data={orf}
                  index={index}
                  onClick={onOpenReadingFrameClick}
                  selected={selectedOrf?.every((i, ii) => i === index[ii])}
                />
              );
              orfs++;
              return tr;
            })}
          </>
        ))}
      </tbody>
    </table>
  ) : (
    <div>Loading...</div>
  );
}

export default AminoAcidView;
