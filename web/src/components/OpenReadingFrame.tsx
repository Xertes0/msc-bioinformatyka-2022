export default function OpenReadingFrame(props: { id: number, content: string }) {
    return (
        <div
            id={`orf${props.id}`}
            dangerouslySetInnerHTML={{ __html: props.content }}
            className="card"
            style={{
                marginBottom: "2px",
                borderColor: "grey",
                borderStyle: "solid",
                borderWidth: "1px",
            }}
        ></div>
    )
}
