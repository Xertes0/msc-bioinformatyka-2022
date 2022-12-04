import {useEffect, useState} from 'react';
import './App.css';

// @ts-ignore
import BioModule from "@cxx/biolib/bio.mjs";

function App() {
    const [bioModule, setBioModule] = useState({
        //bio_test() {
        //    return Number();
        //},
    });
    const [bioModuleLoaded, setBioModuleLoaded] = useState(false);

    useEffect(() => {
        setBioModuleLoaded(false);
        BioModule().then((res: any) => {
            setBioModuleLoaded(true);
            setBioModule(res);
        })
    }, []);

    function svgSubmit(event: React.FormEvent<HTMLFormElement>) {
        event.preventDefault();
        // @ts-ignore
        let str = event.target[0].value;
        if (str == "") {
            return;
        }
        // @ts-ignore
        document.getElementById("svgDiv").innerHTML = "<svg width='100%' height='100%' id='aa_svg' xmlns='http://www.w3.org/2000/svg'>" + bioModule.bio_draw_skeletal(str) + "</svg>";
        let svg = document.getElementById("aa_svg");
        // @ts-ignore
        let bbox: SVGRect = svg.getBBox();
        // @ts-ignore
        svg.setAttribute("viewBox", `${bbox.x} ${bbox.y} ${bbox.width} ${bbox.height + bbox.y}`)
    }

    function orfClick(event: React.MouseEvent<HTMLElement>) {
        // @ts-ignore
        if (event.target.tagName != "A") {
            return;
        }
        // @ts-ignore
        document.getElementById("skeletalInput").value = event.target.text;
        // @ts-ignore
        document.getElementById("svgDiv").innerHTML = "<svg width='100%' height='100%' id='aa_svg' xmlns='http://www.w3.org/2000/svg'>" + bioModule.bio_draw_skeletal(event.target.text) + "</svg>";
        let svg = document.getElementById("aa_svg");
        // @ts-ignore
        let bbox: SVGRect = svg.getBBox();
        // @ts-ignore
        svg.setAttribute("viewBox", `${bbox.x} ${bbox.y} ${bbox.width} ${bbox.height + bbox.y}`)
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
                    <div id="orf0" className="card" style={{border: "1px solid grey", marginBottom: "2px"}}>
                    </div>
                    <div id="orf1" className="card" style={{border: "1px solid grey", marginBottom: "2px"}}>
                    </div>
                    <div id="orf2" className="card" style={{border: "1px solid grey", marginBottom: "2px"}}>
                    </div>
                </div>
            </div>
            <div className="card">
                <h1>Draw a skeletal formula</h1>
                {
                    bioModuleLoaded &&
                    <div className="card">
                        <form onSubmit={svgSubmit}>
                            <label>
                                Sequence<br></br>
                                <input id="skeletalInput" type="text"></input>
                            </label>
                        </form>
                    </div>
                }
                <div className="card" id="svgDiv"
                     style={{height: "45vh", maxWidth: "1000vw", margin: "auto", backgroundColor: "white"}}>
                </div>
            </div>
        </div>
    )
}

export default App;
