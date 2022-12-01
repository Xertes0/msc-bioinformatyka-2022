import {useEffect, useState} from 'react';
import reactLogo from './assets/react.svg';
import './App.css';

// @ts-ignore
import BioModule from '@cxx/biolib/bio.mjs';

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
        let str = event.target[0].value;
        if(str == "") { return; }
        document.getElementById("svgDiv").innerHTML = "<svg width='100%' height='100%' id='aa_svg' xmlns='http://www.w3.org/2000/svg'>" + bioModule.bio_draw_skeletal(str) + "</svg>";
        let svg = document.getElementById("aa_svg");
        let bbox: SVGRect = svg.getBBox();
        svg.setAttribute("viewBox", `${bbox.x} ${bbox.y} ${bbox.width} ${bbox.height + bbox.y}`)
    }

    return (
        <div className="App">
            <h1>Draw a skeletal formula</h1>
            {
                bioModuleLoaded ?
                <div className="card">
                    <form onSubmit={svgSubmit}>
                        <label>
                            Sequence<br></br>
                            <input type="text"></input>
                        </label>
                    </form>
                </div>
                    : ""
            }
            <div className="card" id="svgDiv" style={{width:"75vw", margin:"auto", backgroundColor:"white"}}>
            </div>
        </div>
    )
}

export default App;
