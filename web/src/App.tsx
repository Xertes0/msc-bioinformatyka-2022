import React, { useEffect, useState } from "react";
import "./App.css";

// @ts-ignore
import BioModule from "@cxx/biolib/bio.mjs";

function App() {
    const [bioModule, setBioModule] = useState({});
    const [bioModuleLoaded, setBioModuleLoaded] = useState(false);

    useEffect(() => {
        setBioModuleLoaded(false);
        BioModule().then((res: any) => {
            setBioModuleLoaded(true);
            setBioModule(res);
        })
    }, []);

    function orfClick(event: React.MouseEvent<HTMLElement>) {
        // @ts-ignore
        if (event.target.tagName != "A") {
            return;
        }
        // @ts-ignore
        window.open(`skeletal.html?formula=${event.target.text}`, '_blank', 'noopener noreferrer');
    }

    function transSubmit(event: React.FormEvent<HTMLFormElement>) {
        event.preventDefault();
        // @ts-ignore
        let str = event.target[0].value;
        if (str == "") {
            return;
        }
        // @ts-ignore
        let orfs = bioModule.bio_translate(
            str,
            (index: Number) => {
                return [`<a id='proteinSeq${index}'>`, "</a>"];
            }
        );
        // @ts-ignore
        document.getElementById("orf0").innerHTML = orfs[0];
        // @ts-ignore
        document.getElementById("orf1").innerHTML = orfs[1];
        // @ts-ignore
        document.getElementById("orf2").innerHTML = orfs[2];
    }

    return (
        <div className="App">
            <div className="card">
                <h1>Translate a sequence</h1>
                {
                    bioModuleLoaded &&
                    <div className="card">
                        <form onSubmit={transSubmit}>
                            <label>
                                Sequence<br></br>
                                <input type="text"></input>
                            </label>
                        </form>
                    </div>
                }
                <div onClick={orfClick}>
                    <div id="orf0" className="card" style={{ border: "1px solid grey", marginBottom: "2px" }}>
                    </div>
                    <div id="orf1" className="card" style={{ border: "1px solid grey", marginBottom: "2px" }}>
                    </div>
                    <div id="orf2" className="card" style={{ border: "1px solid grey", marginBottom: "2px" }}>
                    </div>
                </div>
            </div>
        </div>
    )
}

export default App;
