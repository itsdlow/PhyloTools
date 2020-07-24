clc
clear
name='hrv';%hrv/beta_globin/influenza_1163
filename=strcat(name,'.fasta');
[IDs, seqs] = fastaread(filename);
taxanum = length(IDs);

F=[];
for c=2:3
    amino='ARNDCEQGHILKMFPSTWYV';
    a=amino';
    b='';
    K=[];
    for d=1:(c-1)
        e=1;
        for i=1:20^d
            for j=1:20
                b(e,:)=strcat(a(i,:),amino(j));
                e=e+1;
            end
        end
    a=b;
    b='';
    end
    K=a;
    n=20^c;
    
    S=[];
    for l=1:1:taxanum
        seq='';
        seq=seqs{l};
        len=length(seq)-c+1;
        R=[];
        for t=1:1:n
            A=K(t,:);
            C=strfind(seq,A);
            D=length(C);
            
            if D~=0
                E=[D mean(C) sum((C-mean(C)).^2)/(D*len)];
            else
                E=[0 0 0];
            end
            
            R=[R E];    
        end
        S(l,:)=R;
    end
    F=[F S];    
end

measure='cosine';%cosine/euclidean
M=squareform(pdist(F,measure));

outputfilename=strcat('kmer_natural_vector_for_',name,'.meg');
num_of_seq=taxanum;
outfile=fopen(outputfilename, 'w');
fprintf(outfile, '#mega\n');
fprintf(outfile, '!Title: TEST;\n');
fprintf(outfile, '!Format DataType=Distance DataFormat=LowerLeft NTaxa=%d;\n', num_of_seq);
fprintf(outfile, '\n');
for k = 1 : num_of_seq
    fprintf(outfile, '[%d] #%s\n', k, IDs{k});
end
fprintf(outfile, '\n');
for j = 2 : num_of_seq
    fprintf(outfile, '[%d]   ', j);
    for k = 1 : (j-1) 
        fprintf(outfile, ' %8f', M(j, k));
    end
    fprintf(outfile, '\n');
end
fprintf(outfile, '\n');
fclose(outfile);
