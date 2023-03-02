import "./App.css";
import React, { useEffect, useState } from "react";

import OpenReadingFrame from "./components/OpenReadingFrame";
import BioModuleLoad from "@cxx/biolib/bio.mjs";
import SequenceInput from "./SequenceInput";

function App() {
    const [bioModule, setBioModule] = useState<null | BioModule>(null);
    const [orfs, setOrfs] = useState([]);

    useEffect(() => {
        BioModuleLoad().then((res) => {
            setBioModule(res);
        })
    }, []);

    function orfClick(event: React.MouseEvent<HTMLDivElement>) {
        let element = event.target as HTMLDivElement;
        if (element.tagName != "A") {
            return;
        }
        window.open(`skeletal.html?formula=${element.innerText}`, '_blank', 'noopener noreferrer');
    }

    function submit(sequence: string) {
        if (!bioModule) return;
        if (!sequence) return;

        setOrfs(bioModule.translate(
            sequence,
            (index: Number) => [`<a id='proteinSeq${index}'>`, "</a>"]
        ));
    }

    return (
        <div className="App">
            <div className="card">
                <h1>Translate a sequence</h1>
                { bioModule && <div className="card"> <SequenceInput onSubmit={submit} /></div> }
                <div onClick={orfClick}>{ orfs.map((orf, index) => <OpenReadingFrame key={index} id={index} content={orf}/>) }</div>
            </div>
        </div>
    )
}

export default App;
