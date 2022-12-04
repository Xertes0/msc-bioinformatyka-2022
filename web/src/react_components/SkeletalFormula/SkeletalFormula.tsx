import {useParams} from "react-router-dom";
import BioModule from '@cxx/biolib/bio.mjs';
import {useEffect, useState} from "react";

export default function SkeletalFormula() {
    const {formula} = useParams();
    // @ts-ignore
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

    if (!bioModuleLoaded) {
        return <div>Loading...</div>
    }
    // @ts-ignore
    document.getElementById("svgDiv").innerHTML = "<svg width='100%' height='100%' id='aa_svg'" + " xmlns='http://www.w3.org/2000/svg'>" + bioModule.bio_draw_skeletal(formula) + "</svg>";
    let svg = document.getElementById("aa_svg");
    // @ts-ignore
    let bbox: SVGRect = svg.getBBox();
    // @ts-ignore
    svg.setAttribute("viewBox", `${bbox.x} ${bbox.y} ${bbox.width} ${bbox.height + bbox.y}`)
    return (
        <div className={"SkeletalFormula"}>
            <h1>Skeletal Formula</h1>
            <div id="svgDiv">
                <svg width='100%' height='100%' id='aa_svg'
                     xmlns='http://www.w3.org/2000/svg'> {bioModule.bio_draw_skeletal(formula)}</svg>
            </div>
        </div>
    )
}